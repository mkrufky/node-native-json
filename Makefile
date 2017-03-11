TOPLEVEL ?= $(dir $(lastword $(MAKEFILE_LIST)))
CPPLINT ?= $(TOPLEVEL)/cpplint.py
PYTHON ?= python
BUILDTYPE ?= Release
MODULES = parse stringify
SOURCES = $(MODULES:%=test/cpp/%.cpp)
ADDONS = $(MODULES:%=test/build/$(BUILDTYPE)/%.node)

LINT_SOURCES = \
	native-json.h \
	test/cpp/parse.cpp \
	test/cpp/stringify.cpp \
	node_modules/node-gyp/gyp/data/win/large-pdb-shim.cc

FILTER = -whitespace/parens

.PHONY: lint

lint:
	cd $(TOPLEVEL) && $(PYTHON) $(CPPLINT) --filter=$(FILTER) $(LINT_SOURCES)

test: $(ADDONS)
	npm test

forcetest:
	cd test && ../node_modules/.bin/node-gyp rebuild && cd ..
	npm test

$(ADDONS): native-json.h test/binding.gyp $(SOURCES)
	cd test/ && ../node_modules/.bin/node-gyp rebuild
