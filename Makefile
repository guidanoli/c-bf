IN = squares
OUT = $(addsuffix .c, $(IN))

$(OUT): bf

%.c: %.b
	./bf $< $@
	$(CC) $@ -o $<.out

bf: bf.c
	$(CC) $^ -o $@

clean:
	rm -rf *.out $(OUT)

.PHONY: bf clean
