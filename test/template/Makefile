
all:
	./cxxtest_make

clean:
	rm -f runner* *.gcno *.gcov *.gcda TEST*.xml .kdbgrc.runner

dist_clean: clean
	rm -f tags
tags:
	rm -f tags
	ctags-exuberant -aR ../../include/ ../../source/AudioMixer ../../source/AudioClient .
