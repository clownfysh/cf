.PHONY: force
compiler = gcc
machine = $(shell uname -m)
uname = $(shell uname)
os = $(strip $(uname))
ifeq ($(os),Linux)
	os = linux
	platform = -DCF_PLATFORM_LINUX
endif

ifeq ($(MAKECMDGOALS),release)
	build = -DCF_BUILD_RELEASE
	debugging = -DNDEBUG
	optimization = -O3
	mode_identifier = \#
	warnings = -Wall -Werror
else ifeq ($(MAKECMDGOALS),demo)
	build = -DCF_BUILD_DEMO
	debugging = -DNDEBUG
	optimization = -O3
	mode_identifier = ~
	warnings = -Wall -Werror
else
	build = -DCF_BUILD_DEVELOPMENT
	debugging = -g
	optimization = -O0
	mode_identifier = +
	warnings = -Wall -Werror
endif

library_paths = -L.
ifeq ($(os),linux)
  library_paths += -L/usr/local/lib -L/usr/lib
endif
include_paths = -I$(CF) -I$(CF)/.. -I$(CF_C_STANDARD) -I$(CF_POSTGRESQL)
libraries = -lcrypto -ljpeg -lm -lncurses -lpq -lpthread -luuid
link_options = -ansi $(build) $(platform) $(debugging) $(optimization) $(warnings) $(include_paths) -D_GNU_SOURCE $(library_paths) $(libraries)
compile_options = -ansi $(build) $(platform) $(debugging) $(optimization) $(warnings) $(include_paths) -D_GNU_SOURCE

object_bases = $(shell find . -name "*.c" | grep -v "\.test\.c" | grep -v "\.impl\." | grep -v "\.bin\.c" | sed 's/\(.*\)\..*/\1/')
object_sources := $(foreach object_base, $(object_bases), $(object_base).c)
#object_headers := $(foreach object_base, $(object_bases), $(object_base).h)
objects := $(foreach object_base, $(object_bases), $(object_base).o)
external_objects := $(shell external-objects)
object_tests := $(foreach object_base, $(object_bases), $(object_base).test)
#object_test_sources := $(foreach object_base, $(object_bases), $(object_base).test.c)
programs = $(shell find . -name "*.bin.c" | sed 's/\(.*\)\..*/\1/')
#program_sources := $(foreach program, $(programs), $(program).c)

development: _all

demo: clean _all

release: clean _all

stats:
	@stats

tags:
	@make-etags

test: _test

tests: _tests

todo:
	@show-todos

clean:
	@find . -name "*~" -exec rm {} \;
	@find . -name ".*~" -exec rm {} \;
	@find . -name "*.o" -exec rm {} \;
	@find . -name "*.test" -exec rm {} \;
	@find . -name "*.bin" -exec rm {} \;
	@find . -name ".makedepend.bak" -exec rm {} \;
	@find . -name ".etags" -exec rm {} \;
	@echo "" > .makedepend
	@clean

help:
	@echo "make         : make objects, programs"
	@echo "make test    : make tests, run tests"
	@echo "make tests   : make tests"
	@echo "make demo    : make demo objects, programs"
	@echo "make release : make release objects, programs"
	@echo "make tags    : make emacs tags (in .etags)"
	@echo "make clean   : clean"
	@echo "make stats   : show code statistics"
	@echo "make todo    : show todo items from code"

_all: _init _objects _programs

_debug:
	@echo $(object_bases)

_init:
	@makedepend -f.makedepend $(include_paths) $(object_sources)

_objects: $(objects)

_test: _tests
	@run-tests $(object_tests)

_tests: $(objects) $(object_tests)

_programs: $(programs)

_run_tests:
	@run-tests $(object_tests)

%.o: %.c
	@quiet-execute "$(mode_identifier) $@" "$(compiler) $< -o $@ -c $(compile_options)"

%.test: %.test.c force
	@quiet-execute "$(mode_identifier) $@" "$(compiler) $< -o $@ $(objects) $(external_objects) $(compile_options) $(link_options)"

%.bin: %.bin.c force
	@quiet-execute "$(mode_identifier) $@" "$(compiler) $< -o $@ $(objects) $(external_objects) $(compile_options) $(link_options)"

include .makedepend
