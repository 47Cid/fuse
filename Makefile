build: main.c 
	gcc main.c fops.c -o fs `pkg-config fuse --cflags --libs`

clean:
	rm fs
