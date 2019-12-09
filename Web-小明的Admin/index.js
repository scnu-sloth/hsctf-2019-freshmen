const sercet = require("./sercet");
const http = require("http");
const fs = require("fs");
const qs = require("querystring");

const port = parseInt(process.env.PORT, 10) || 4000;

http
  .createServer(function(req, res) {
    if (req.url === "/" || !req.url) {
      fs.createReadStream("./index.html", {
        encoding: "utf8"
      }).pipe(res);
    }
    if (req.url === "/attack") {
      if (req.method === "POST") {
        let body = "";

        req.on("data", function(data) {
          body += data;
          // Too much POST data, kill the connection!
          if (body.length > 1e6) req.connection.destroy();
        });
        req.on("end", function() {
          const post = qs.parse(body);
          const ip = req.socket.remoteAddress;
          const port = req.socket.localPort;
          console.log(`${ip} ${post.input}`);
          let input;
          try {
            try {
              input = JSON.parse(post.input);
            } catch (e) {
              input = {};
            }
            delete input.isAdmin; // you can't do bad thing anyway
            console.log(input);
            console.log({}.__proto__);
            const user = merge(
              {
                name: "default",
                sex: "default"
              },
              input
            );
            if (user.isAdmin === true) {
              res.write(sercet.flag);
              delete {}.__proto__.isAdmin;
            } else {
              res.write(JSON.stringify(user));
            }
          } catch (e) {
            console.error(e);
            res.statusCode = 500;
            res.write("500 error");
          }
          res.end();
        });
      }
    }
  })
  .listen(port);
console.log("Listening on " + port);

function merge(dst, src) {
  for (key in src) {
    if (Array.isArray(dst[key]) && Array.isArray(src[key])) {
      // concatenate arrays that are values of the same object key
      object1[key] = dst[key].concat(src[key]);
    } else if (typeof dst[key] === "object" && typeof src[key] === "object") {
      // deep merge src into dst
      dst[key] = merge(dst[key], src[key]);
    } else {
      dst[key] = src[key];
    }
  }
  return dst;
}
