BB_HOME=/usr/bb
install:
	mkdir -p $(BB_HOME)/etc/init.d
	mkdir -p $(BB_HOME)/var/log
	mkdir -p $(BB_HOME)/var/run
	install -m644 httpd.conf $(BB_HOME)/etc
	install -m755 init.d/apachectl $(BB_HOME)/etc/init.d
