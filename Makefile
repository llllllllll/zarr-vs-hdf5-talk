GRAPHS := $(wildcard images/*.dot)
IMAGES := $(GRAPHS:.dot=.png)

slides.pdf: slides.tex $(IMAGES)
	pdflatex -shell-escape -interaction=nostopmode $< -o $@

.PHONY: present
present: slides.pdf
	evince -s $<

.PHONY: open
open: slides.pdf
	evince $<

%.png: %.dot
	dot -Tpng $< -o $@

.PHONY: clean
clean:
	rm -f slides.aux \
		slides.nav \
		slides.snm \
		slides.toc \
		slides.log \
		slides.vrb \
		slides.pdf \
		$(IMAGES)
	rm -rf _minted_slides
