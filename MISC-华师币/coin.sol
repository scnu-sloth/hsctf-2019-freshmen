pragma solidity >=0.4.22;

// SCNU Coin
contract ScnuCoin {

    // Account structure
    struct Account {
        uint studentId;
        uint balance;
    }

    mapping(address => Account) public accounts;
    address private owner;

    constructor() public {
        owner = msg.sender;
    }

    // Register
    function register(uint studentId_) public {
        require(
            studentId_ != 0,
            "Invalid student ID"
        );

        require(
            accounts[msg.sender].studentId == 0,
            "You are registered!"
        );

        accounts[msg.sender].studentId = studentId_;
        accounts[msg.sender].balance = 500;
    }

    function registerWithInvitor(uint studentId_, address invitor_) public {
        require(
            studentId_ != 0,
            "Invalid student ID"
        );

        require(
            accounts[msg.sender].studentId == 0,
            "You are registered!"
        );

        require(
            accounts[invitor_].studentId != 0,
            "Invitor does not exist!"
        );


        accounts[invitor_].balance += 3000;
        uint balance_ = 3000;

        accounts[msg.sender].studentId = studentId_;
        accounts[msg.sender].balance = balance_ + 500;
    }

    // Transfer
    function transfer(address to_, uint value) public {
        require(
            accounts[to_].studentId != 0,
            "The target account does not exist!"
        );
        require(
            accounts[msg.sender].balance >= value,
            "You have not enough money!"
        );

        accounts[msg.sender].balance -= value;
        accounts[to_].balance += value;
    }

    // Donate
    function donate(uint value) public {
        require(
            accounts[msg.sender].balance >= value,
            "You have not enough money!"
        );

        accounts[msg.sender].balance -= value;
    }


    function random() private view returns (uint) {
       return uint(keccak256(block.timestamp, block.difficulty))%251;
   }

    // Lucky gacha
    function gacha() public returns(uint) {
        require(
            accounts[msg.sender].balance >= 1000,
            "You have not enough money to buy gacha!"
        );

        accounts[msg.sender].balance -= 1000;

        uint res = random() % 100;
        uint win = 0;
        if (res == 0) {
            win = 8888;
        } else if (res <= 5) {
            win = 2333;
        } else if (res <= 15) {
            win = 1688;
        } else if (res <= 30) {
            win = 1000;
        }

        accounts[msg.sender].balance += win;

        return win;
    }

    // Test
    function test() public {
        require(
            owner == msg.sender,
            "You are not the owner!"
        );

        accounts[msg.sender].balance += 900000;
    }

    event Flag (address indexed buyer, string flag);

    // Buy flag
    function buyFlag() public {
        require(
            accounts[msg.sender].balance > 888888,
            "You must pay 888888 HSC"
        );

        accounts[msg.sender].balance -= 888888;

        // flag: HSCTF{H@pPy_ETher-Neeetw0rk!}
        // https://www.qqxiuzi.cn/bianma/kaisamima.php
        // param: 3
        emit Flag(msg.sender, "Caesar!KVFWI{K@sSb_HWkhu-Qhhhwz0un!}");
    }
}
