BB_HOME=/usr/bb
all:
install:
	mkdir -p $(BB_HOME)/etc/init.d
	mkdir -p $(BB_HOME)/etc/httpd.conf.d
	mkdir -p $(BB_HOME)/var/log
	mkdir -p $(BB_HOME)/var/run
	install -m644 httpd.conf $(BB_HOME)/etc
	install -m644 nginx.conf $(BB_HOME)/etc
	install -m755 init.d/apachectl $(BB_HOME)/etc/init.d
	install -m755 init.d/bbircd $(BB_HOME)/etc/init.d
