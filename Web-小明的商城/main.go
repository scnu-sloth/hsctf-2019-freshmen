package main

import (
	"fmt"
	"html/template"
	"log"
	"math"
	"math/rand"
	"net/http"
	"os"
	"strings"
	"sync"
	"time"

	"encoding/json"
)

const bigSize = math.MaxInt16 << 1

const price = 500
const initBalance = 999

const FLAG_FMT = "flag{oh_y0u_w0n_t1e_r4ce_!__%s__!}"
const FLAG_REGEX = "^flag{oh_y0u_w0n_t1e_r4ce_!__[A-Za-z-@#$]{15}__!}$"

type Item struct {
	Balance int
	Count   int
}

type personMap struct {
	mu sync.Mutex

	m [bigSize]*Item
}

var pMap personMap

func setNameCookie(w http.ResponseWriter, name string) {
	http.SetCookie(w, &http.Cookie{
		Name:     "name",
		Value:    name,
		Expires:  time.Now().Add(time.Hour * 1),
		HttpOnly: true,
	})
}

func hash(str string) int64 {
	var ret int64
	for _, x := range str {
		ret = ret + int64(x)
		ret = ret << 1
		ret = ret * int64(x)
		ret = ret >> 2
		ret = ret % math.MaxInt64
	}
	if ret < 0 {
		ret = -ret
	}
	ret = ret % bigSize
	return ret
}

func main() {

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		// log 404 path
		if r.URL.Path != "/" {
			if r.URL.Path != "/favicon.ico" {
				log.Printf("[NOTFOUND] %s %s", strings.Split(r.RemoteAddr, ":")[0], r.URL.Path)
			}
			http.NotFound(w, r)
			return
		}
		name, err := r.Cookie("name")
		if err != nil {
			http.Redirect(w, r, "/start", 302)
			return
		}
		index := hash(name.Value)
		pMap.mu.Lock()
		if pMap.m[index] == nil {
			http.Redirect(w, r, "/start", 302)
			pMap.mu.Unlock()
			return
		}
		item := *pMap.m[index]
		pMap.mu.Unlock()
		tpl, err := template.ParseFiles("./pages/index.html")
		if err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
		}
		if err := tpl.Execute(w, struct {
			Count   int
			Balance int
		}{2 - item.Count, item.Balance}); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
		}
	})

	http.HandleFunc("/start", func(w http.ResponseWriter, r *http.Request) {
		var name string
		c, err := r.Cookie("name")
		if err == nil {
			name = c.Value
		} else {
			log.Printf("[Start] %s %s\n", strings.Split(r.RemoteAddr, ":")[0], name)
			name = randomName(15)
			setNameCookie(w, name)
		}
		index := hash(name)
		pMap.mu.Lock()
		item := Item{
			Balance: initBalance,
			Count:   0,
		}
		pMap.m[index] = &item
		pMap.mu.Unlock()
		http.ServeFile(w, r, "./pages/start.html")
	})

	http.HandleFunc("/buy1", func(w http.ResponseWriter, r *http.Request) {
		// !!!!RACE
		//pMap.mu.Lock()
		//defer pMap.mu.Unlock()
		cookie, err := r.Cookie("name")
		if err != nil {
			http.Redirect(w, r, "/start", 302)
			return
		}
		name := cookie.Value
		index := hash(name)
		var item Item
		if pMap.m[index] == nil {
			item = Item{
				Balance: initBalance,
				Count:   0,
			}
			pMap.m[index] = &item
		} else {
			item = *pMap.m[index]
		}
		balance := item.Balance
		if balance < price {
			fmt.Fprint(w, buildJsonResp("restart", "余额不足"))
		} else {
			balance = balance - price

			item.Balance = balance
			pMap.m[index].Count++
			time.Sleep(time.Millisecond * 300)
			pMap.m[index].Balance = balance
			if pMap.m[index].Count >= 2 {
				flagStr := fmt.Sprintf(FLAG_FMT, name)
				fmt.Fprint(w, buildJsonResp("success", flagStr))
				log.Printf("[GetFlag] %s %s %s \n", strings.Split(r.RemoteAddr, ":")[0], name, flagStr)
			} else {
				//s := buildJsonResp("success", "成功买到 id:"+strconv.FormatInt(index, 10)+" count: "+strconv.Itoa(pMap.m[index].Count))
				s := buildJsonResp("success", "成功买到一个")
				fmt.Fprint(w, s)
			}
		}
	})

	port := os.Getenv("PORT")
	if port == "" {
		port = "4000"
	}
	fmt.Printf("Lisenting on prot %s\n", port)
	http.ListenAndServe(":"+port, nil)
}

func buildJsonResp(code, msg string) string {
	ret := make(map[string]string)
	ret["msg"] = msg
	ret["code"] = code
	j, _ := json.Marshal(ret)
	return string(j)
}

var alphabet = []rune("abcdefghijkmlnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWKYZ-@#$")

func randomName(length int) string {
	if length == 0 {
		length = 10
	}
	for {
		var sb strings.Builder
		sb.Grow(length)
		rand.Seed(time.Now().UnixNano())
		dictLen := len(alphabet)
		for i := 0; i < length; i++ {
			sb.WriteRune(alphabet[rand.Intn(dictLen)])
		}
		s := sb.String()
		pMap.mu.Lock()
		defer pMap.mu.Unlock()
		if pMap.m[hash(sb.String())] == nil {
			return s
		}
	}
}
