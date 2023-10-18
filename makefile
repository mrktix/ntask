test:
	clang++ \
		-fsanitize=address \
		-fno-omit-frame-pointer -g \
		-std=c++17 \
		-lncurses \
		src/back/*.cpp src/main/*.cpp \
		-o bin/test

install:
	clang++ \
		-fsanitize=address \
		-std=c++17 \
		-lncurses \
		-s \
		src/*.cpp -o ~/.local/bin/norgtask

asan:
	clang++ \
		-fsanitize=address \
		-fno-omit-frame-pointer -g \
		-std=c++17 \
		-lncurses \
		src/*.cpp -o bin/asan

tsan:
	clang++ \
		-fsanitize=thread \
		-fno-omit-frame-pointer -g \
		-std=c++17 \
		-lncurses \
		src/*.cpp -o bin/tsan

msan:
	clang++ \
		-fsanitize=memory \
		-fno-omit-frame-pointer -g \
		-std=c++17 \
		-lncurses \
		src/*.cpp -o bin/msan
