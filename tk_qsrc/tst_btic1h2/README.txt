What this is:
An experimental codec, BTIC1H, intended mostly for low computational complexity use-cases. While acceptable image quality and compression are goals, they exist at a lower priority than making it reasonably fast.

The design also aims to allow the encoder at least some freedom in terms of how it will do things, although some things are not flexible (such as its use of Adaptive-Rice encoding). While adaptive rice isn't the cheapest option, it was a compromise between the properties I wanted (in one of the main possible use-cases, reliable delivery of a Huffman table could not be assured, and use of fixed-Huffman would be a more effort).

The design directly descends from BTIC1G, which had a similar overall design, but which relied on bit-packing and raw bytes instead of a Rice-coded bitstream.


It is assumed that the codec would be modified and adapted as needed for the use-case, though versions with incompatible changes should probably use a different FOURCC.

Note here that BTIC1H basically means:
  BTIC: BGBTech Image Coded
  1: Family 1, generally used for blocky VQ/ColorCell designs.
  H: Generation H.

In some cases, a number which follows representing the major version.
Generally this is done for versions which introduce "breaking changes".


What it is not:
* Intended to compete with or replace mainstream video codecs;
* Intended to be any sort of "standard".


How it compares with other codecs:
Most other mainstream codecs are based on the use of the DCT transform, and focus primarily on bitrate/quality rather than speed. These lead to a different set of design tradeoffs.

Instead, this codec is primarily based on operation similar to that in Color Cell Compression.


Presumed Build Environment (Windows):
  MSVC Command Prompt with MinGW or Cygwin tools in PATH.
  'makecodec.bat' should then compile the codec.

The file 'inst_inf.bat' is an install batch. It may need to be modified depending on the source path for the codec and similar.

Note that the code


Legal Blob:
<--
Copyright (C) 2015 by Brendan G Bohannon

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-->
