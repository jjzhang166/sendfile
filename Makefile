sendfile:sendfile.c
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f sendfile 
