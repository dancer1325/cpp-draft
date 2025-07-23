==========================
C++ Standard Draft Sources
==========================

* goal
  * sources / generate C++ standard's drafts
    * ❌!= ISO publication❌

## Getting Started | 
### Mac OS X

* install 
  * [MacTeX distribution](http://tug.org/mactex/)
  * if you are | slow network,
    * get [BasicTeX package](http://tug.org/mactex/morepackages.html)
  * `sudo tlmgr install latexmk isodate substr relsize ulem fixme rsfs extract layouts enumitem l3packages l3kernel imakeidx splitindex xstring`

### Debian-based Systems

Install the following packages::

   sudo apt-get install latexmk texlive-latex-recommended texlive-latex-extra texlive-fonts-recommended lmodern

### Fedora

Install the following packages::

   dnf install latexmk texlive texlive-isodate texlive-relsize texlive-ulem texlive-fixme texlive-extract texlive-l3kernel texlive-l3packages texlive-splitindex texlive-imakeidx

### Arch Linux

Install the following packages::

   pacman -S make texlive-latexextra texlive-binextra texlive-plaingeneric texlive-fontsrecommended

### Microsoft Windows

Install Perl (for example, using a `Cygwin installation <https://cygwin.com/install.html>`_ and adding perl.
See `sample instructions <https://bennierobinson.com/programming/2016/01/24/perl-windows-2016.html>`_ for more details)

Install `MiKTeX <https://miktex.org/download>`_

Alternatively, you can `install WSL <https://learn.microsoft.com/en-us/windows/wsl/install>`_
and a Linux distribution,
and follow the instructions for Linux above.

## Instructions

* | [source](source),
  * `make`
    * generate a std.pdf

## Generated input files

To regenerate figures from .dot files, run::

   make <pdfname>

For example::

   make figvirt.pdf

## Verifying input and output when making changes

While in the ``source`` directory,
you can check for common mistakes such as misaligned comments with::

    ../tools/check-source.sh

After typesetting the document,
you can also check the output for further problems with::

    ../tools/check-output.sh

GitHub Actions will also run these checks when you create a pull request,
but it's often faster if you run them locally first.
