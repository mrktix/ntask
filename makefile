test:
	clang++ \
		-fsanitize=address \
		-fno-omit-frame-pointer -g \
		-std=c++17 \
		-lncurses \
		-o bin/test \
		src/back/*.cpp src/main/*.cpp

install:
	clang++ \
		-fsanitize=address \
		-std=c++17 \
		-lncurses \
		-s \
		-o ~/.local/bin/norgtask \
		src/back/*.cpp src/main/*.cpp

asan:
	clang++ \
		-fsanitize=address \
		-fno-omit-frame-pointer -g \
		-std=c++17 \
		-lncurses \
		-o bin/asan \
		src/back/*.cpp src/main/*.cpp

tsan:
	clang++ \
		-fsanitize=thread \
		-fno-omit-frame-pointer -g \
		-std=c++17 \
		-lncurses \
		-o bin/tsan \
		src/back/*.cpp src/main/*.cpp

msan:
	clang++ \
		-fsanitize=memory \
		-fno-omit-frame-pointer -g \
		-std=c++17 \
		-lncurses \
		-o bin/msan \
		src/back/*.cpp src/main/*.cpp
