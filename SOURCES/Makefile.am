## ea-php-cli - Makefile.am                  Copyright 2016 cPanel, Inc.
##                                                  All rights Reserved.
## copyright@cpanel.net                                http://cpanel.net
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
## 
##     http://www.apache.org/licenses/LICENSE-2.0
## 
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.

ACLOCAL_AMFLAGS=-I m4

EXTRA_DIST = LICENSE bootstrap README.apt

SUBDIRS = include src tests

COVERAGE_INFO_FILE = $(top_builddir)/coverage.info
COVERAGE_REPORT_DIR =  $(top_builddir)/coverage

coverage.info: coverage-zero.info coverage-report.info
	$(LCOV) --add-tracefile coverage-zero.info --add-tracefile coverage-postcheck.info --output-file $(COVERAGE_INFO_FILE)

coverage: coverage-reset coverage-report

coverage-reset: coverage-remove-report coverage-remove-maps

coverage-report: coverage-build-sources coverage-zero-report coverage-run-check coverage-build-report coverage-merge-report coverage-build-html

coverage-tools-present:
	@if test ! -e $(GCOV); then \
		echo "Cannot find $(GCOV). Please install gcov."; \
		exit 1; \
	else \
		echo "$(GCOV) present."; \
	fi
	@if test ! -e $(LCOV); then \
		echo "Cannot find $(LCOV). Please install lcov."; \
		exit 1; \
	else \
		echo "$(LCOV) present."; \
	fi
	@if test ! -e $(GENHTML); then \
		echo "Cannot find $(GENHTML). Please install genhtml."; \
		exit 1; \
	else \
		echo "$(GENHTML) present."; \
	fi

coverage-remove-report:
	-rm -rf $(COVERAGE_INFO_FILE)
	-rm -rf $(COVERAGE_REPORT_DIR)

coverage-zero-report: coverage-tools-present
	-$(LCOV) --no-external --capture --initial --directory $(abs_top_srcdir)/src --output-file coverage-zero.info

coverage-zero-html:
	-$(GENHTML) --prefix $(abs_top_builddir) --ignore-errors source $(COVERAGE_INFO_FILE) --legend --title "`git rev-parse --abbrev-ref HEAD`" --output-directory=$(COVERAGE_REPORT_DIR)

coverage-remove-maps:
	@if xargs --version 2>&1 1>/dev/null; then \
		find $(top_builddir) -name "*.gcno" | xargs --no-run-if-empty rm; \
	else \
		find $(top_builddir) -name "*.gcno" | xargs rm; \
	fi

coverage-build-sources: coverage-tools-present
	$(MAKE) $(AM_FLAGS) clean;
	$(MAKE) $(AM_FLAGS) \
		CFLAGS="$(CFLAGS) $(COVERAGE_CFLAGS) $(COVERAGE_OPTFLAGS)" \
		LDFLAGS="$(LDFLAGS) $(COVERAGE_LDFLAGS)" \
		LIBS="$(LIBS) $(COVERAGE_LIBS)"

coverage-run-check: coverage-tools-present
	$(MAKE) $(AM_MAKEFLAGS) -k check \
		CFLAGS="$(CFLAGS) $(COVERAGE_CFLAGS) $(COVERAGE_OPTFLAGS)" \
		LDFLAGS="$(LDFLAGS) $(COVERAGE_LDFLAGS)" \
		LIBS="$(LIBS) $(COVERAGE_LIBS)"
	
coverage-build-report: coverage-tools-present
	$(LCOV) --no-external --capture --directory "$(top_builddir)/src" --output-file coverage-postcheck.info

coverage-merge-report: coverage-tools-present
	$(LCOV) --add-tracefile coverage-zero.info --add-tracefile coverage-postcheck.info --output-file $(COVERAGE_INFO_FILE)

coverage-build-html: coverage-tools-present
	-$(GENHTML) --prefix $(abs_top_builddir) --ignore-error source $(COVERAGE_INFO_FILE) --legend --title "php-cli branch: `git rev-parse --abbrev-ref HEAD` (`git rev-parse HEAD`)" --output-directory=$(COVERAGE_REPORT_DIR)
