SOURCE = ./
BUILD = build/
MAIN_CLASS = game.Launcher

all:
		@echo "test"

compile: clean
		@mkdir -p $(BUILD)
		@gcc -Wall -g -pedantic $(SOURCE) -o $(BUILD)/$(SOURCE)
		@echo "Compilation completed"

run:
		@java -cp $(BUILD) $(MAIN_CLASS)

clean:
		@rm -rf $(BUILD)