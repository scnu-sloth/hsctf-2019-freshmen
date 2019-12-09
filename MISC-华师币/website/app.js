const RPC_URL = 'http://coin.scnu.syp.xyz/rpc/';
const CONTRACT_ADDR = '0x417F089593c2b7446689f15465B4Ac7cEe0ce9C0';

(function () {
    const provider = new ethers.providers.JsonRpcProvider(RPC_URL);
    let wallet;

    const $ = (tag) => {
        return document.getElementById(tag);
    };

    if (!localStorage.pk) {
        const wallet_ = ethers.Wallet.createRandom();
        localStorage.pk = wallet_.privateKey;
        wallet = wallet_.connect(provider);
    } else {
        wallet = new ethers.Wallet(localStorage.pk, provider);
    }

    $('txt-address').innerText = wallet.address;

    const contract = new ethers.Contract(CONTRACT_ADDR, ABI, wallet);

    function hexCharCodeToStr(hexCharCodeStr) {
        var trimedStr = hexCharCodeStr.trim();
        var rawStr =
            trimedStr.substr(0,2).toLowerCase() === "0x"
                ?
                trimedStr.substr(2)
                :
                trimedStr;
        var len = rawStr.length;
        if(len % 2 !== 0) {
            alert("Illegal Format ASCII Code!");
            return "";
        }
        var curCharCode;
        var resultStr = [];
        for(var i = 0; i < len;i = i + 2) {
            curCharCode = parseInt(rawStr.substr(i, 2), 16); // ASCII Code Value
            resultStr.push(String.fromCharCode(curCharCode));
        }
        return resultStr.join("");
    }

    provider.getCode(CONTRACT_ADDR).then(v => console.log(hexCharCodeToStr(v)));
    
    const getAccountInfo = () => {
        contract.accounts(wallet.address)
            .then(v => {
                $('txt-balance').innerText = v.balance.toString();
                $('txt-student-id').innerText = v.studentId.eq(0) ? '未注册' : v.studentId.toString();

                if (v.studentId.eq(0)) {
                    $('page-reg').style.display = null;
                    $('page-main').style.display = 'none';
                } else {
                    $('page-reg').style.display = 'none';
                    $('page-main').style.display = null;
                }
            })
    };

    getAccountInfo();
    setInterval(getAccountInfo, 5000);

    const $btn_reg = $('btn-reg');
    $btn_reg.onclick = () => {
        const studentId = parseInt(($('input-reg-student-id').value || '').trim());
        const invitor = ($('input-reg-invitor').value || '').trim();

        if (isNaN(studentId) || studentId <= 0) {
            return alert('学号必须填写，并且格式需要正确');
        }

        if (invitor && /^0x[0-9a-fA-F]{16,}$/.test(invitor) === false) {
            return alert('邀请者的钱包地址格式不合法');
        }

        let promise;
        if (invitor) {
            promise = contract.registerWithInvitor(studentId, invitor);
        } else {
            promise = contract.register(studentId)
        }

        $btn_reg.disabled = true;
        $btn_reg.innerText = '正在注册，需要矿工确认..';
        promise.then(v => {
            return v.wait();
        }).then(v => {
            alert('注册成功！');

            $btn_reg.disabled = false;
            $('page-reg').style.display = 'none';
            $('page-main').style.display = null;
        }).catch(err => {
            $btn_reg.disabled = false;
            $btn_reg.innerText = '注册';
            alert('遇到错误！请稍后再试');
        })
    };

    const $btn_buy_flag = $('btn-buy-flag');
    const $btn_gacha = $('btn-gacha');
    const $btn_donate = $('btn-donate');
    const $btn_transfer = $('btn-transfer');

    const setBtnDisabled = (disabled) => {
        $btn_buy_flag.disabled = !!disabled;
        $btn_gacha.disabled = !!disabled;
        $btn_donate.disabled = !!disabled;
        $btn_transfer.disabled = !!disabled;
    };

    $btn_transfer.onclick = () => {
        const to = ($('input-transfer-to').value || '').trim();
        const amount = parseInt(($('input-transfer-amount').value || '').trim());

        if (isNaN(amount) || amount <= 0) {
            return alert('转账金额必须大于0');
        }
        if (/^0x[0-9a-fA-F]{16,}$/.test(to) === false) {
            return alert('收款人的地址不合法');
        }

        setBtnDisabled(true);
        contract.transfer(to, amount).then(v => {
            return v.wait();
        }).then(v => {
            alert('转账成功！');

            setBtnDisabled(false);
        }).catch(err => {
            setBtnDisabled(false);
            alert('您的钱包余额不足！');
        })
    };

    $btn_donate.onclick = () => {
        const amount = parseInt(($('input-donate-amount').value || '').trim());

        if (isNaN(amount) || amount <= 0) {
            return alert('捐款金额必须大于0');
        }

        setBtnDisabled(true);
        contract.donate(amount).then(v => {
            return v.wait();
        }).then(v => {
            alert('捐款成功！感谢您对华师的支持！！');

            setBtnDisabled(false);
        }).catch(err => {
            setBtnDisabled(false);
            alert('你的钱包余额不足');
        })
    };

    $btn_gacha.onclick = () => {
        setBtnDisabled(true);
        contract.gacha().then(v => {
            return v.wait();
        }).then(v => {
            alert('购买乐透成功！如果中奖将会自动兑换');

            setBtnDisabled(false);
        }).catch(err => {
            setBtnDisabled(false);
            alert('没钱了QAQ，买不了大乐透');
        })
    };

    $btn_buy_flag.onclick = () => {
        setBtnDisabled(true);
        contract.buyFlag().then(v => {
            return v.wait();
        }).then(v => {
            alert('购买Flag成功！');

            setBtnDisabled(false);
        }).catch(err => {
            setBtnDisabled(false);
            alert('您没有资格购买Flag');
        })
    };

    contract.on(contract.filters.Flag(wallet.address, null), (fromAddress, toAddress, value, event) => {
        $('txt-flag').innerText = value.args.flag;
    });
})();