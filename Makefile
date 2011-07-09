
all: ece554_project

.PHONY: ece554_project
ece554_project:
	scons

.PHONY: tags
tags:
	rm -rf ./tags && ctags-exuberant -aR ./ ./include ./source

.PHONY: clean
clean:
	scons -c
