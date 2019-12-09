FROM golang:1.12-alpine3.10 AS builder
WORKDIR /usr/app/
RUN sed -i 's/dl-cdn.alpinelinux.org/mirrors.tuna.tsinghua.edu.cn/g' /etc/apk/repositories\
 && apk add --no-cache --update build-base
COPY main.go .
RUN GOOS=linux GOARCH=amd64 go build -ldflags "-s -w" -a main.go
#RUN go build -ldflags "-linkmode external -extldflags -static" -a main.go

FROM alpine:3.10.1
WORKDIR /usr/app/
COPY --from=builder /usr/app/main /usr/app/buyrace
COPY pages ./pages
ENTRYPOINT ["/usr/app/buyrace"]