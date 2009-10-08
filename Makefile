BB_HOME=/usr/bb
install:
	make -C etc install
	make -C src/libbbrpc install
	make -C src/libapicommon install
	make -C src/libbbdrizzle all
	make -C src/libbbevqueue install
	make -C src/logapi all
	make -C src/libbbirc install
	make -C src/bbircd install
	make -C src/bbui install

clean:
	make -C src/libbbrpc clean
	make -C src/libapicommon clean
	make -C src/libbbdrizzle clean
	make -C src/libbbevqueue clean
	make -C src/logapi clean
	make -C src/libbbirc clean
	make -C src/bbircd clean
