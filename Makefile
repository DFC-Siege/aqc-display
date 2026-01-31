BUILD_DIR = build
PROJECT_NAME = display

build:
	@mkdir -p $(BUILD_DIR)
	@if [ ! -f "$(BUILD_DIR)/CMakeCache.txt" ]; then \
		cmake -S . -B $(BUILD_DIR) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON; \
		ln -sf $(BUILD_DIR)/compile_commands.json .; \
	fi
	@cmake --build $(BUILD_DIR) --target $(PROJECT_NAME)

flash:
	@picotool load -x $(BUILD_DIR)/src/$(PROJECT_NAME).uf2 -f

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all build flash clean
