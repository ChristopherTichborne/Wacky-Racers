all: guide.pdf

%.pdf: %.tex hardware.tex software.tex troubleshooting.tex openocd.tex
	pdflatex -shell-escape $<
	pdflatex -shell-escape $<

clean:
	-rm -rf *.out *.aux *.log

push:
	git push
