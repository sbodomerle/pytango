################################################################################
##
## This file is part of PyTango, a python binding for Tango
## 
## http://www.tango-controls.org/static/PyTango/latest/doc/html/index.html
##
## Copyright 2011 CELLS / ALBA Synchrotron, Bellaterra, Spain
## 
## PyTango is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
## 
## PyTango is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public License
## along with PyTango.  If not, see <http://www.gnu.org/licenses/>.
##
################################################################################

#
# PyTango documentation build configuration file, created by
# sphinx-quickstart on Fri Jun  5 14:31:50 2009.
#
# This file is execfile()d with the current directory set to its containing dir.
#
# Note that not all possible configuration values are present in this
# autogenerated file.
#
# All configuration values have a default; values that are commented out
# serve to show the default.

import sys, os
import re
import PyTango

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
sys.path.append(os.path.abspath('sphinxext'))

# -- General configuration -----------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be extensions
# coming with Sphinx (named 'sphinx.ext.*') or your custom ones.
extensions = ['sphinx.ext.pngmath',
              'sphinx.ext.autodoc',
              'sphinx.ext.doctest',
              'sphinx.ext.graphviz',
              'sphinx.ext.intersphinx',
              'ipython_console_highlighting',
              'tango_console_highlighting']

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix of source filenames.
source_suffix = '.rst'

# The encoding of source files.
#source_encoding = 'utf-8'

# The master toctree document.
master_doc = 'index'

# General information about the project.
project = u'PyTango'
copyright = u'2012, ALBA - CELLS'
copyright = u"""Except where otherwise noted, content on this site is
licensed under a Creative Commons Attribution 3.0 License"""

#Ideally we would like to put the following html code for copyright... but how?
'''<a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/es/"><img alt="Creative Commons License" style="border-width:0" src="http://i.creativecommons.org/l/by-sa/3.0/es/88x31.png" /></a><br /><span xmlns:dc="http://purl.org/dc/elements/1.1/" href="http://purl.org/dc/dcmitype/Text" property="dc:title" rel="dc:type">Tau Documentation</span> by <span xmlns:cc="http://creativecommons.org/ns#" property="cc:attributionName">CELLS - ALBA</span> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/3.0/es/">Creative Commons Attribution-Share Alike 3.0 Spain License</a>.'''

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.
version = '.'.join(PyTango.Release.version.split('.')[:2])
# The full version, including alpha/beta/rc tags.
release = PyTango.Release.version

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#language = None

# There are two options for replacing |today|: either, you set today to some
# non-false value, then it is used:
#today = ''
# Else, today_fmt is used as the format for a strftime call.
#today_fmt = '%B %d, %Y'

# List of documents that shouldn't be included in the build.
#unused_docs = []

# List of directories, relative to source directory, that shouldn't be searched
# for source files.
exclude_trees = ['_build']

# The reST default role (used for this markup: `text`) to use for all documents.
#default_role = None

# If true, '()' will be appended to :func: etc. cross-reference text.
#add_function_parentheses = True

# If true, the current module name will be prepended to all description
# unit titles (such as .. function::).
#add_module_names = True

# If true, sectionauthor and moduleauthor directives will be shown in the
# output. They are ignored by default.
#show_authors = False

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'
pygments_style = 'tango_console_highlighting.TangoStyle'

# A list of ignored prefixes for module index sorting.
#modindex_common_prefix = []


# -- Options for HTML output ---------------------------------------------------

# The theme to use for HTML and HTML Help pages.  Major themes that come with
# Sphinx are currently 'default' and 'sphinxdoc'.
html_theme = 'default'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#html_theme_options = {}

# Add any paths that contain custom themes here, relative to this directory.
#html_theme_path = []

# The name for this set of Sphinx documents.  If None, it defaults to
# "<project> v<release> documentation".
#html_title = None

# A shorter title for the navigation bar.  Default is the same as html_title.
#html_short_title = None

# The name of an image file (relative to this directory) to place at the top
# of the sidebar.
html_logo = 'logo.png'

# The name of an image file (within the static path) to use as favicon of the
# docs.  This file should be a Windows icon file (.ico) being 16x16 or 32x32
# pixels large.
#html_favicon = None

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# If not '', a 'Last updated on:' timestamp is inserted at every page bottom,
# using the given strftime format.
#html_last_updated_fmt = '%b %d, %Y'

# If true, SmartyPants will be used to convert quotes and dashes to
# typographically correct entities.
#html_use_smartypants = True

# Custom sidebar templates, maps document names to template names.
#html_sidebars = {}

# Additional templates that should be rendered to pages, maps page names to
# template names.
#html_additional_pages = {}

# If false, no module index is generated.
#html_use_modindex = True

# If false, no index is generated.
#html_use_index = True

# If true, the index is split into individual pages for each letter.
#html_split_index = False

# If true, links to the reST sources are added to the pages.
#html_show_sourcelink = True

# If true, "Created using Sphinx" is shown in the HTML footer. Default is True.
#html_show_sphinx = True

# If true, "(C) Copyright ..." is shown in the HTML footer. Default is True.
#html_show_copyright = True

# If true, an OpenSearch description file will be output, and all pages will
# contain a <link> tag referring to it.  The value of this option must be the
# base URL from which the finished HTML is served.
#html_use_opensearch = ''

# If nonempty, this is the file name suffix for HTML files (e.g. ".xhtml").
#html_file_suffix = ''

# Output file base name for HTML help builder.
htmlhelp_basename = 'PyTangodoc'


# -- Options for LaTeX output --------------------------------------------------

# The paper size ('letter' or 'a4').
latex_paper_size = 'a4'

# The font size ('10pt', '11pt' or '12pt').
latex_font_size = '10pt'

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title, author, documentclass [howto/manual]).
latex_documents = [
  ('index', 'PyTango.tex', u'PyTango Documentation',
   u'PyTango team', 'manual'),
]

# The name of an image file (relative to this directory) to place at the top of
# the title page.
latex_logo = 'logo.png'

# For "manual" documents, if this is true, then toplevel headings are parts,
# not chapters.
#latex_use_parts = False

# Additional stuff for the LaTeX preamble.
#latex_preamble = ''

# Documents to append as an appendix to all manuals.
#latex_appendices = []

# If false, no module index is generated.
#latex_use_modindex = True

# -- Options for reference to other documentation ------------------------------

intersphinx_mapping = {
    'http://docs.python.org/dev': None,
    'http://docs.scipy.org/doc/scipy/reference' : None,
    'http://docs.scipy.org/doc/numpy' : None,
}

def copy_spaces(origin):
    r = ''
    for x in xrange(len(origin)):
        if origin[x] in (' ', '\t'):
            r += origin[x]
        else:
            return r
    return r

def type_to_link(tipus):
        if tipus[:9] == 'sequence<' and tipus[-1:] == '>':
            return 'sequence<' + type_to_link(tipus[9:-1]) + '>'
        #elif tipus in dir(PyTango):
        else:
            return ':class:`' + tipus + "`"
        #else:
        #    return tipus

def type_to_pytango_link(tipus):
        if tipus[:9] == 'sequence<' and tipus[-1:] == '>':
            return 'sequence<' + type_to_link(tipus[9:-1]) + '>'
        elif tipus in dir(PyTango):
            return ':class:`' + tipus + "`"
        else:
           return tipus

def possible_type_to_link(text):
    if len(text) and text[0] == '(' and text[-1] == ')':
        return '(' + type_to_link(text[1:-1]) +')'
    return text

def parse_typed_line(line):
    spacesSplit = line.strip().split(' ')
    first = spacesSplit[0].strip()
    return possible_type_to_link(first) + ' ' + ' '.join(spacesSplit[1:])

def parse_parameters(line):
    spaces = copy_spaces(line)
    miniLine = line.strip()

    if miniLine[:2] != '- ':
        return line

    spl = miniLine[2:].split(':', 1)

    assert(len(spl) == 2)
    
    return spaces + ':' + spl[0].strip() + ': ' + parse_typed_line(spl[1])


def parse_bullet_with_type(line):
    spaces = copy_spaces(line)
    miniLine = line.strip()

    if miniLine[:2] not in ['- ', '* ']:
        return line

    spl = miniLine.split(':', 1)

    if len(spl) != 2:
        return line

    return spaces + spl[0] + ': ' + parse_typed_line(spl[1])


def parse_throws(line):
    words = re.split('(\W+)', line)
    assert(line == ''.join(words))
    return ''.join(map(type_to_pytango_link, words))


# http://codedump.tumblr.com/post/94712647/handling-python-docstring-indentation
def docstring_to_lines(docstring):
    if not docstring:
        return []
    lines = docstring.expandtabs().splitlines()

    # Determine minimum indentation (first line doesn't count):
    indent = sys.maxint
    for line in lines[1:]:
        stripped = line.lstrip()
        if stripped:
            indent = min(indent, len(line) - len(stripped))

    # Remove indentation (first line is special):
    trimmed = [lines[0].strip()]
    if indent < sys.maxint:
        for line in lines[1:]:
            trimmed.append(line[indent:].rstrip())

    # Strip off trailing and leading blank lines:
    while trimmed and not trimmed[-1]:
        trimmed.pop()
    while trimmed and not trimmed[0]:
        trimmed.pop(0)
    return trimmed

def search_ONLY_signature(name, text):
    lines = docstring_to_lines(text)

    # There should be ONE signature and must be the FIRST text
    # Signature is the ONLY starting at position 0

    signatureLine = None

    for ln in xrange(len(lines)):
        line = lines[ln]

        if len(line.strip()) and line[0] != ' ':
            parentesis = line.split('(', 1)
            fname = parentesis[0].strip()
            if len(parentesis)==2 and fname == name.rsplit('.',1)[1]:
                if signatureLine is not None: # More than one signature!
                    return None
                signatureLine = ln
            else:
                return None # There's a text as FIRST text that's NOT the signature!

    if signatureLine is None:
        return None

    return lines[signatureLine]

def split_signature(text):
    if text is None:
        return None
    
    # split "fname(params)", "returntype"
    ops = text.split('->')
    if len(ops) != 2:
        return None
    
    # get rid of "fname"
    params = ops[0].strip()
    ret_type = ops[1].strip()
    p = params.find('(')
    if p < 0:
        return None
    params = params[p:]
    return params, ret_type
    

    
_with_only_one_signature_methods = {}

def __reformat_lines(app, what, name, obj, options, lines):
    global _with_only_one_signature_methods
    if what != 'method':
        for ln in xrange(len(lines)):
            lines[ln] = parse_bullet_with_type(lines[ln])
        return

    toinsert = []
    parsingParameters = False
    parsingThrows = False

    toinsert.append((0, ""))
    
    for ln in xrange(len(lines)):
        line = lines[ln]

        if len(line) and line[0] != ' ':
            if name in _with_only_one_signature_methods:
                # This method has one and only one signature. So it will
                # be displayed by sphinx, there's no need for us to fake
                # it here...
                lines[ln] = ""
            else:
                parentesis = line.split('(', 1)
                fname = parentesis[0].strip()
                if len(parentesis)==2 and fname == name.rsplit('.',1)[1]:
                    sg = split_signature(line)
                    if sg is not None:
                        # Main lines are like small titles (**bold**):
                        lines[ln] = '**' + fname +'** *' + sg[0] + '* **->** ' + type_to_link(sg[1])
                        # Add an ENTER after the title, to make a different
                        # paragraph. So if I have 2 signatures, there's no problem
                        # with it...
                        toinsert.append((ln+1, ""))

                    ## Main lines are like small titles (**bold**):
                    #lines[ln]='**' + line.strip() + '**'
                    ## Add an ENTER after the title, to make a different
                    ## paragraph. So if I have 2 signatures, there's no problem
                    ## with it...
                    #toinsert.append((ln+1, ""))


        # Mark the "New in this version" lines...
        if line.strip()[:14] == "New in PyTango":
            lines[ln] = copy_spaces(lines[ln]) + "*" + line.strip() + "*"
            parsingParameters = False
            parsingThrows = False

        # Look for special control_words
        # To replace the actual syntax: "Return   : something"
        # with the one understood by reStructuredText ":Return: something"
        spl = line.strip().split(':', 1)
        control_word = spl[0].strip()
            
        if ((len(spl) != 2)
            or (control_word not in ["Parameters", "Return", "Throws", "Example", "See Also" ]) ):
                if parsingParameters:
                    lines[ln] = parse_parameters(line)
                elif parsingThrows:
                    lines[ln] = parse_throws(line)
                continue

        parsingParameters = False
        parsingThrows = False
        spaces = copy_spaces(line)

        # The Example control word is even more special. I will put
        # the contents from the following line into a code tag (::)
        if control_word == 'Example':
            lines[ln] = spaces + ":" + control_word + ": " + spl[1]
            toinsert.append((ln+1, ""))
            toinsert.append((ln+1, spaces + ' ::'))
            toinsert.append((ln+1, ""))
        elif control_word == 'Parameters':
            lines[ln] = spaces + ":Parameters:" + parse_parameters(spl[1])
            parsingParameters = True
        elif control_word == 'Return':
            lines[ln] = spaces + ":Return: " + parse_typed_line(spl[1])
        elif control_word == "Throws":
            lines[ln] = spaces + ":Throws:" + parse_throws(spl[1])
            parsingThrows = True
        else:
            lines[ln] = spaces + ":" + control_word + ": " + spl[1]

    for x in xrange(len(toinsert)-1, -1, -1):
        pos, txt = toinsert[x]
        lines.insert(pos, txt)


def __process_signature(app, what, name, obj, options, signature, return_annotation):
    global _with_only_one_signature_methods
    if what != 'method':
        return
    sg = split_signature(search_ONLY_signature(name, obj.__doc__))
    if sg is not None:
        _with_only_one_signature_methods[name] = True
        return sg
    return (signature, return_annotation)

def setup(app):
    # sphinx will call these methods when he finds an object to document.
    # I want to edit the docstring to adapt its format to something more
    # beautiful.
    # I also want to edit the signature because boost methods have no
    # signature. I will read the signature from the docstring.
    # The order sphinx will call it is __process_signature, __reformat_lines.
    # And it is important because I keep some information between the two
    # processes
    # Problem is __process_signature works great with python methods...
    # but is not even called for methods defined by boost. So, as it is,
    # is useless now.
    
    #app.connect('autodoc-process-signature', __process_signature)
    app.connect('autodoc-process-docstring', __reformat_lines)
