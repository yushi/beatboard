BB_ETC_HOME=/usr/bb/etc
install:
	mkdir -p $(BB_ETC_HOME)/init.d
	install -m644 httpd.conf $(BB_ETC_HOME)
	install -m644 init.d/apachectl $(BB_ETC_HOME)/init.d
