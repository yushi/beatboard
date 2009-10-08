BB_HOME=/usr/bb
install:
	make -C etc install
	make -C src/libbbrpc install BB=1
	make -C src/libapicommon install BB=1
	make -C src/libbbdrizzle all BB=1
	make -C src/libbbevqueue install
	make -C src/logapi all BB=1
	make -C src/libbbirc install
	make -C src/bbircd install
	make -C src/bbui install

clean:
	make -C src/libbbrpc clean
	make -C src/libapicommon clean
	make -C src/libbbdrizzle clean BB=1
	make -C src/libbbevqueue clean
	make -C src/logapi clean BB=1
	make -C src/libbbirc clean
	make -C src/bbircd clean
