OUT = $(addsuffix .out, $(wildcard scripts/*.b))

all: bf $(OUT)

%.b.out: %.b
	./bf $< $<.c
	$(CC) $<.c -o $@

bf: bf.c
	$(CC) $^ -o $@

clean:
	cd scripts && rm -rf *.b.c *.b.out

.PHONY: all clean
