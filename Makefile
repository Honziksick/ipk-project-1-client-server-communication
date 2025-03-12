################################################################################
#                                                                              #
# Project:      OMEGA L4 Scanner                                               #
# University:   Faculty of Information Technology, BUT                         #
# Subject:      IPK: Computer Communications and Networks                      #
#                                                                              #
# File:         Makefile                                                       #
# Author:       Jan Kalina <xkalinj00>                                         #
#                                                                              #
# Created:      12.03.2025                                                     #
# Last edit:    12.03.2025                                                     #
#                                                                              #
# Description:  This Makefile is used for compiling the project "OMEGA L4      #
#               Scanner" for the IPK course. Besides building, the Makefile    #
#               also serves to automate other tasks such as generating         #
#               documentation, cleaning project directories, packaging the     #
#               project for submission, etc. This Makefile is inspired by      #
#               Makefiles I created for previous projects at BUT FIT (e.g.,    #
#               for the IVS and IFJ courses).                                  #
#                                                                              #
################################################################################

################################################################################
#                                                                              #
#                 BASIC SETTINGS AND DEFINITIONS FOR MAKEFILE                  #
#                                                                              #
################################################################################

###                                   ###
#  Basic configuration of the Makefile  #
###                                   ###

# Project name
EXECUTABLE = ipk-l4-scan

# Name of the ZIP archive for project submission
PACK_NAME = xkalinj00

# ANSI sequences for colors
COLOR_RESET = \033[0m
COLOR_RED = \033[0;31m
COLOR_GREEN = \033[0;32m
COLOR_BLUE = \033[0;36m
COLOR_YELLOW = \033[0;33m
COLOR_MAGENTA = \033[0;35m


###                                        ###
#  Switches for running the $(MAKE) command  #
###                                        ###

# Run 'make' in silent mode (without event output)
$(VERBOSE)SILENTOPT = -s

# Definition of a constant to disable selected targets (for submission)
#DISABLE_TARGETS ?= true

###                                           ###
#  Definition of paths for some inputs/outputs  #
###                                           ###

# Directory for generating documentation
DOC_DIR = doc


###                                                                    ###
#  Declaration of paths to directories intended for project compilation  #
###                                                                    ###

# Path to the directory with source files for the compiler
SRC_DIR = src

# Path to the directory with tests
TEST_DIR = test

# Directories for placing built files
BUILD_DIR = build

# Directory with the prepared project for packaging
PACK_DIR = pack
ARCHIVE_DIR = $(PACK_DIR)/$(PACK_NAME)


################################################################################
#                                                                              #
#                                MAIN COMMANDS                                 #
#                                                                              #
################################################################################

# The '.PHONY' command indicates that the following commands are never considered as files
.PHONY: all build run help clean doc pack clean-all clean-build clean-exec clean-doc \
        clean-pack pack-prepare install-dev-dep install-help-dep install-doc-dep \
        install-pack-dep update-dep

### MC # all: # Performs the build of the entire compiler intended for deployment
all: build

### MC # build: # Builds the compiler for the "Team xkalinj00"
build:
	@$(MAKE) clean-build
	@$(MAKE) clean-exec
	@cmake -S . -B build
	@cmake --build build

### MC # run: Runs the executable with print help argument
run: build
	./$(EXECUTABLE) -h

# Definition of shortcuts for command categories
CATEGORIES := MC C P DEV

### MC # help: # Prints help for using the Makefile
help:
ifndef DISABLE_TARGETS
	@$(MAKE) $(SILENTOPT) install-help-dep
endif
	@{ \
	for CATEGORY in $(CATEGORIES); do \
		case $$CATEGORY in \
		"MC") FULL_CAT="Main Commands";; \
		"C") FULL_CAT="Clean (special)";; \
		"P") FULL_CAT="Pack (special)";; \
		"DEV") FULL_CAT="Install Dependencies";; \
		esac; \
		echo "$(COLOR_YELLOW)$$FULL_CAT:$(COLOR_RESET)"; \
		grep -E "^### $$CATEGORY # [a-zA-Z0-9_\-]+:.*?# .*$$" $(MAKEFILE_LIST) | \
		sort -f | \
		awk 'BEGIN {FS = ":.*?# "}; \
		{ \
			gsub(/^### [A-Z]+ # /, "", $$1); \
			split($$2, lines, "\\\\n"); \
			printf "$(COLOR_BLUE)%-30s$(COLOR_RESET) %s\n", $$1, lines[1]; \
			for (i = 2; i <= length(lines); i++) { \
				printf "$(COLOR_BLUE)%-30s$(COLOR_RESET) %s\n", "", lines[i]; \
			} \
		}'; \
		echo ""; \
	done; \
	} | less -R

### MC # clean: # Runs 'clean-all' in developer mode od 'clean-build' + 'clean-doc' in submission mode
ifndef DISABLE_TARGETS
clean: clean-all
else
clean: clean-build clean-doc
endif

### MC # doc: # Generates project documentation into the `doc` directory
ifndef DISABLE_TARGETS
doc:
	@$(MAKE) $(SILENTOPT) install-doc-dep
	$(MAKE) $(SILENTOPT) clean-doc
	doxygen Doxyfile
	cd $(DOC_DIR)/html && grep -v 'target="_self">resources\|target="_self">doc' files.html > temp.html && mv temp.html files.html
	@echo '<html><head><meta http-equiv="refresh" content="0; url=html/index.html"></head></html>' > $(DOC_DIR)/documentation.html
	@echo -e "$(COLOR_YELLOW)Do you want to open the HTML documentation in the main system browser? (y/n): $(COLOR_RESET)"
	@bash -c 'read -t 5 -p "" choice; \
	if [ "$$choice" = "y" ]; then \
		if grep -qEi "(Microsoft|WSL)" /proc/version &> /dev/null; then \
			cmd.exe /C start $(DOC_DIR)/documentation.html; \
		else \
			xdg-open $(DOC_DIR)/documentation.html; \
		fi \
	fi'
else
doc:
	$(MAKE) $(SILENTOPT) clean-doc
	doxygen Doxyfile
	@echo '<html><head><meta http-equiv="refresh" content="0; url=./html/index.html"></head></html>' > $(DOC_DIR)documentation.html
endif

### MC # pack: # Creates a ZIP archive with files intended for submission
ifndef DISABLE_TARGETS
pack:
	@$(MAKE) $(SILENTOPT) install-pack-dep
	$(MAKE) $(SILENTOPT) clean-pack
	mkdir -p $(PACK_DIR)
	$(MAKE) $(SILENTOPT) pack-prepare
	@echo ""
	@cd $(ARCHIVE_DIR) && zip -qr ../$(PACK_NAME) ./
else
pack:
	@echo "$(COLOR_RED)The 'pack' target is disabled for project submission.$(COLOR_RESET)"
endif


################################################################################
#                                                                              #
#                        SPECIALIZED 'CLEAN' COMMANDS                          #
#                                                                              #
################################################################################

### C # clean-all: # Removes all created files (build, doc, executable, archive, ...)
clean-all: clean-build clean-doc clean-pack

### C # clean-build: # Removes the 'build' directory
clean-build:
	rm -rf $(BUILD_DIR)

### C # clean-exec: # Removes the executable
clean-exec:
	rm -f $(EXECUTABLE)

### C # clean-doc: # Removes generated content of the 'doc' directory
ifndef DISABLE_TARGETS
clean-doc:
	find $(DOC_DIR) -mindepth 1 ! -path '$(DOC_DIR)/resources*' ! -path '$(DOC_DIR)/raw*' -delete || true
else
clean-doc:
	rm -rf $(DOC_DIR)
endif

### C # clean-pack: # Removes the 'pack' directory (including the archive)
ifndef DISABLE_TARGETS
clean-pack:
	rm -rf $(PACK_DIR)
else
clean-pack:
	@echo "$(COLOR_RED)The 'clean-pack' target is disabled for project submission.$(COLOR_RESET)"
endif


################################################################################
#                                                                              #
#                    PACKAGING THE PROJECT FOR SUBMISSION INTO '.ZIP'          #
#                                                                              #
################################################################################

### P # pack-prepare: # Copies all necessary files to the 'pack/xkalinj00' directory
ifndef DISABLE_TARGETS
pack-prepare:
	@{ \
		missing_files=0; \
		if [ -d "$(SRC_DIR)" ]; then \
			rsync -a --include '*/' --include '*.cpp' --exclude '*' --exclude '*/' \
			--prune-empty-dirs ./ $(ARCHIVE_DIR)/; \
		else \
			echo "$(COLOR_RED)\nError: The directory "$(SRC_DIR)" does not exist.$(COLOR_RESET)"; \
		fi; \
		if [ -d "$(SRC_DIR)" ]; then \
			rsync -a --include '*/' --include '*.hpp' --exclude '*' --exclude '*/' \
			--prune-empty-dirs ./ $(ARCHIVE_DIR)/; \
		else \
			echo "$(COLOR_RED)\nError: The directory "$(SRC_DIR)" does not exist.$(COLOR_RESET)"; \
		fi; \
		if [ -f "Makefile" ]; then \
			rsync -a Makefile $(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		if [ -f "CMakeLists.txt" ]; then \
			rsync -a CMakeLists.txt $(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		if [ -f "Doxyfile" ]; then \
			rsync -a Doxyfile $(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		if [ -f "README.md" ]; then \
			rsync -a README.md $(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		if [ -f "CHANGELOG.md" ]; then \
			rsync -a CHANGELOG.md $(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		if [ -f "LICENSE" ]; then \
			rsync -a LICENSE $(ARCHIVE_DIR)/; \
		else \
			missing_files=1; \
		fi; \
		echo "$(COLOR_GREEN)\nList of copied files:$(COLOR_RESET)"; \
		find $(PACK_DIR) -type f -printf "$(COLOR_GREEN)%p$(COLOR_RESET)\n"; \
		if [ "$$missing_files" -eq 1 ]; then \
			echo "$(COLOR_RED)\nList of missing files:$(COLOR_RESET)"; \
		fi; \
		if [ ! -f "$(ARCHIVE_DIR)/Makefile" ]; then \
			echo "$(COLOR_RED)Error: The file "Makefile" was not copied.$(COLOR_RESET)"; \
		fi; \
		if [ ! -f "$(ARCHIVE_DIR)/CMakeLists.txt" ]; then \
        	echo "$(COLOR_RED)Error: The file "CMakeLists.txt" was not copied.$(COLOR_RESET)"; \
        fi; \
		if [ ! -f "$(ARCHIVE_DIR)/Doxyfile" ]; then \
			echo "$(COLOR_RED)Error: The file "Doxyfile" was not copied.$(COLOR_RESET)"; \
		fi; \
		if [ ! -f "$(ARCHIVE_DIR)/README.md" ]; then \
			echo "$(COLOR_RED)Error: The file "README.md" was not copied.$(COLOR_RESET)"; \
		fi; \
		if [ ! -f "$(ARCHIVE_DIR)/CHANGELOG.md" ]; then \
			echo "$(COLOR_RED)Error: The file "CHANGELOG.md" was not copied.$(COLOR_RESET)"; \
		fi; \
		if [ ! -f "$(ARCHIVE_DIR)/LICENSE" ]; then \
			echo "$(COLOR_RED)Error: The file "LICENSE" was not copied.$(COLOR_RESET)"; \
		fi; \
	}
else
pack-prepare:
	@echo "$(COLOR_RED)The 'pack-prepare' target is disabled for project submission.$(COLOR_RESET)"
endif

################################################################################
#                                                                              #
#                    TARGETS FOR INSTALLING NECESSARY TOOLS                    #
#                                                                              #
################################################################################

### DEV # install-dev-dep: # Installs dependencies needed for using all 'Makefile' functions
ifndef DISABLE_TARGETS
install-dev-dep: update-dep install-help-dep install-doc-dep install-pack-dep
else
install-dev-dep:
	@echo "$(COLOR_RED)The 'install-dev-dep' target is disabled for project submission.$(COLOR_RESET)"
endif

### DEV # install-help-dep: # Installs dependencies needed for printing 'Makefile' help
ifndef DISABLE_TARGETS
install-help-dep:
	@dpkg -s less >/dev/null 2>&1 || (echo "Installing less" && sudo apt-get install less)
else
install-help-dep:
	@echo "$(COLOR_RED)The 'install-help-dep' target is disabled for project submission.$(COLOR_RESET)"
endif

### DEV # install-doc-dep: # Installs dependencies needed for generating documentation
ifndef DISABLE_TARGETS
install-doc-dep:
	@dpkg -s doxygen >/dev/null 2>&1 || (echo "Installing doxygen" && sudo apt-get install doxygen)
else
install-doc-dep:
	@echo "$(COLOR_RED)The 'install-doc-dep' target is disabled for project submission.$(COLOR_RESET)"
endif

### DEV # install-pack-dep: # Installs dependencies needed for project packaging
ifndef DISABLE_TARGETS
install-pack-dep:
	@dpkg -s rsync >/dev/null 2>&1 || (echo "Installing rsync" && sudo apt-get install rsync)
	@dpkg -s zip >/dev/null 2>&1 || (echo "Installing zip" && sudo apt-get install zip)
else
install-pack-dep:
	@echo "$(COLOR_RED)The 'install-pack-dep' target is disabled for project submission.$(COLOR_RESET)"
endif

### DEV # update-dep: # Updates the list of available packages
ifndef DISABLE_TARGETS
update-dep:
	sudo apt-get update -y
else
update-dep:
	@echo "$(COLOR_RED)The 'dev-update-dep' target is disabled for project submission.$(COLOR_RESET)"
endif
