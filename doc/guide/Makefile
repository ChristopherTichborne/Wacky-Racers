all: guide.pdf

%.pdf: %.tex hardware.tex software.tex troubleshooting.tex openocd.tex
	pdflatex $<
	pdflatex $<

clean:
	-rm -rf *.out *.aux *.log

push:
	git push
