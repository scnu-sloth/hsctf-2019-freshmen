# 欢迎您使用华师币

## 概括

- 出题人：Payne
- 类型：MISC
- 关键字：以太坊、智能合约、Solidity
- Flag：`HSCTF{H@pPy_ETher-Neeetw0rk!}`

## 题目内容

欢迎您使用华师币！国内首创校园实名区块链电子代币～

## 项目结构

- coin.sol: Solidity智能合约源代码
- website/: 网页客户端
- website/abi.js: 智能合约的ABI
- website/app.js: 客户端源代码
- website/index.html: 网页源代码

> 请使用 Remix 编译智能合约, 配置规则:
> - Compiler: 0.4.26
> - EVM Version: byzantium
> 
> 或者使用[这个配置好的链接](https://remix.ethereum.org/#optimize=true&evmVersion=byzantium&version=soljson-v0.4.26+commit.4563c3fc.js)。

## Writeup

这道题有多种解法，下面分别阐述：

### 分析智能合约法

我们可以阅读客户端的源代码，发现网页客户端使用 `ethers.js`，我们很容易找到 [Ethers.js 的官方文档](https://docs.ethers.io/ethers.js/html/index.html)。

然后，我们很容易就发现智能合约的地址，可以写代码提取智能合约的源代码：

```js
provider.getCode(CONTRACT_ADDR).then(v => console.log(v));
```

使用 [https://ethervm.io/decompile](https://ethervm.io/decompile) 分析一波源代码。

找到两个异常的 `PUSH` 指令：

```
02F2    7F    PUSH32 0x436165736172214b564657497b4b407353625f48576b68752d51686868777a30
031A    7F    PUSH32 0x756e217d00000000000000000000000000000000000000000000000000000000
```

解码：

```javascript
function hex2a(hexx) {
    var hex = hexx.toString();//force conversion
    var str = '';
    for (var i = 0; (i < hex.length && hex.substr(i, 2) !== '00'); i += 2)
        str += String.fromCharCode(parseInt(hex.substr(i, 2), 16));
    return str;
}
hex2a('436165736172214B564657497B4B407353625F48576B68752D51686868777A30756E217D');
```

输出：

```
"Caesar!KVFWI{K@sSb_HWkhu-Qhhhwz0un!}"
```

根据提示，使用凯撒解密，偏移为3即可解出。

### 自我部署智能合约法

和前面一个方法一样，你可以获取智能合约的源代码，而且也很容易拿到智能合约的ABI。

那么我们只需要做的就是在本地搭建以太坊私网，并部署该智能合约。

使用部署该智能合约的钱包账号，运行 `test()` 函数，获得 `900000` 华师币。

再用 `bugFlag()` 函数购买 Flag，输出：

```
"Caesar!KVFWI{K@sSb_HWkhu-Qhhhwz0un!}"
```

根据提示，使用凯撒解密，偏移为3即可解出。

### 刷邀请-转账法

根据游戏规则，注册新账号可以获得 `500` 华师币。
每邀请一个用户，双方都可以获得 `3000` 华师币。

因此，每个账号价值 `3500` 华师币。
只需要刷 `254` 个账号就可以刷到足够购买 Flag 的华师币。

再购买 Flag，根据提示，使用凯撒解密，偏移为3即可解出。

### 抽奖法【运气】

根据游戏规则，注册新账号可以获得 `500` 华师币。
每邀请一个用户，双方都可以获得 `3000` 华师币。
每一次抽奖需要 `1000` 华师币。

欧洲人可以靠中奖买 Flag。

> 数学期望是负数，因此完全看人品。

### 偷看交易结果【运气】

监控区块链各个区块的交易，直到遇到了「刷邀请-转账法」大佬提交的 `flag` 函数交易，窃取其中的交易响应。

阅读ABI可以轻易发现，里面有一个Flag事件，非常诡异。看上去像是通知Flag的事件。

> 因为我们知道以太坊是靠日志LOG响应结果的。

那么如何监听一个Flag事件，看代码：

```js
contract.on(contract.filters.Flag(null, null), (fromAddress, toAddress, value, event) => {
    $('txt-flag').innerText = value.args.flag;
});
```
