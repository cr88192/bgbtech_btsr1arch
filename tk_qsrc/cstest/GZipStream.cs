/*
 * GZipStream.cs - Implementation of the
 *		"System.IO.Compression.GZipStream" class.
 *
 * Copyright (C) 2004  Southern Storm Software, Pty Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

namespace System.IO.Compression
{

#if CONFIG_COMPRESSION

using System;
using System.IO;
using ICSharpCode.SharpZipLib.Zip.Compression;
using ICSharpCode.SharpZipLib.Checksums;
using ICSharpCode.SharpZipLib.GZip;

public class GZipStream : Stream
{
	// Internal state.
	private Stream stream;
	private CompressionMode mode;
	private bool leaveOpen;
	private Inflater inflater;
	private Deflater deflater;
	private byte[] buf;
	private Crc32 crc32;
	private bool endOfStream;
	private bool headerDone;

	// Constructors.
	public GZipStream(Stream stream, CompressionMode mode)
			: this(stream, mode, false) {}
	public GZipStream(Stream stream, CompressionMode mode, bool leaveOpen)
			{
				if(stream == null)
				{
					throw new ArgumentNullException("stream");
				}
				if(mode == CompressionMode.Decompress)
				{
					if(!stream.CanRead)
					{
						throw new ArgumentException
							(S._("IO_NotReadable"), "stream");
					}
				}
				else if(mode == CompressionMode.Compress)
				{
					if(!stream.CanWrite)
					{
						throw new ArgumentException
							(S._("IO_NotWritable"), "stream");
					}
				}
				else
				{
					throw new ArgumentException
						(S._("IO_CompressionMode"), "mode");
				}
				this.stream = stream;
				this.mode = mode;
				this.leaveOpen = leaveOpen;
				this.buf = new byte [4096];
				this.crc32 = new Crc32();
				this.endOfStream = false;
				this.headerDone = false;
				if(mode == CompressionMode.Decompress)
				{
					inflater = new Inflater(true);
				}
				else
				{
					deflater = new Deflater(-1, true);
				}
			}

	// Get the base stream that underlies this one.
	public Stream BaseStream
			{
				get
				{
					return stream;
				}
			}

	// Determine if the stream supports reading, writing, or seeking.
	public override bool CanRead
			{
				get
				{
					return (stream != null &&
							mode == CompressionMode.Decompress);
				}
			}
	public override bool CanWrite
			{
				get
				{
					return (stream != null &&
							mode == CompressionMode.Compress);
				}
			}
	public override bool CanSeek
			{
				get
				{
					return false;
				}
			}

	// Get the length of the stream.
	public override long Length
			{
				get
				{
					throw new NotSupportedException(S._("IO_NotSupp_Seek"));
				}
			}

	// Get or set the current seek position within this stream.
	public override long Position
			{
				get
				{
					throw new NotSupportedException(S._("IO_NotSupp_Seek"));
				}
				set
				{
					throw new NotSupportedException(S._("IO_NotSupp_Seek"));
				}
			}

	// Begin an asynchronous read operation.
	public override IAsyncResult BeginRead
				(byte[] buffer, int offset, int count,
				 AsyncCallback callback, Object state)
			{
				if(stream == null)
				{
					throw new ObjectDisposedException
						(S._("Exception_Disposed"));
				}
				if(mode != CompressionMode.Decompress)
				{
					throw new NotSupportedException(S._("IO_NotSupp_Read"));
				}
				return base.BeginRead(buffer, offset, count, callback, state);
			}

	// Wait for an asynchronous read operation to end.
	public override int EndRead(IAsyncResult asyncResult)
			{
				return base.EndRead(asyncResult);
			}

	// Note: The .NET Framework SDK 2.0 version of this class does
	// not have BeginWrite and EndWrite for some inexplicable reason.

	// Close this stream.
	public override void Close()
			{
				if(stream != null)
				{
					if(deflater != null)
					{
						int temp;
						deflater.Finish();
						while(!deflater.IsFinished)
						{
							temp = deflater.Deflate(buf, 0, buf.Length);
							if(temp <= 0)
							{
								if(!deflater.IsFinished)
								{
									throw new IOException
										(S._("IO_Compress_Input"));
								}
								break;
							}
							stream.Write(buf, 0, temp);
						}
						byte[] footer = new byte [8];
						temp = (int)(crc32.Value);
						footer[0] = (byte)temp;
						footer[1] = (byte)(temp << 8);
						footer[2] = (byte)(temp << 16);
						footer[3] = (byte)(temp << 24);
						temp = deflater.TotalIn;
						footer[4] = (byte)temp;
						footer[5] = (byte)(temp << 8);
						footer[6] = (byte)(temp << 16);
						footer[7] = (byte)(temp << 24);
						stream.Write(footer, 0, 8);
					}
					if(!leaveOpen)
					{
						stream.Close();
					}
					stream = null;
					inflater = null;
					deflater = null;
					buf = null;
				}
			}

	// Flush this stream.
	public override void Flush()
			{
				if(stream == null)
				{
					throw new ObjectDisposedException
						(S._("Exception_Disposed"));
				}
			}

	// Read a byte from the underlying stream and update a crc.
	private int ReadByte(Crc32 crc)
			{
				int value = stream.ReadByte();
				if(value >= 0)
				{
					crc.Update(value);
				}
				return value;
			}
	private int ReadByteThrow(Crc32 crc)
			{
				int value = stream.ReadByte();
				if(value >= 0)
				{
					crc.Update(value);
					return value;
				}
				throw new IOException(S._("IO_Decompress_GzipHeader"));
			}

	// Read the gzip header from the base stream.
	private void ReadHeader()
			{
				Crc32 headerCrc = new Crc32();
				int magic1, magic2, flags, temp;

				// Validate the magic number.
				magic1 = ReadByte(headerCrc);
				if(magic1 < 0)
				{
					// The stream is empty, so just report EOF and exit.
					endOfStream = true;
					return;
				}
				magic2 = ReadByte(headerCrc);
				if(magic2 < 0 ||
				   ((magic1 << 8) + magic2) != GZipConstants.GZIP_MAGIC)
				{
					// The magic number does not match.
					throw new IOException(S._("IO_Decompress_GzipHeader"));
				}

				// Check the compression type, which must be 8.
				if(ReadByte(headerCrc) != 0x08)
				{
					throw new IOException(S._("IO_Decompress_GzipHeader"));
				}

				// Read the flags.
				flags = ReadByte(headerCrc);
				if((flags & 0xE0) != 0)
				{
					throw new IOException(S._("IO_Decompress_GzipHeader"));
				}

				// Skip the modification time, extra flags, and OS type.
				for(temp = 0; temp < 6; ++temp)
				{
					ReadByteThrow(headerCrc);
				}

				// Skip the "extra" field.
				if((flags & GZipConstants.FEXTRA) != 0)
				{
					// Get the length of the extra field.
					temp = ReadByteThrow(headerCrc);
					temp += (ReadByteThrow(headerCrc) << 8);

					// Skip the extra field.
					while(temp > 0)
					{
						ReadByteThrow(headerCrc);
						--temp;
					}
				}

				// Skip the "filename" field.
				if((flags & GZipConstants.FNAME) != 0)
				{
					do
					{
						temp = ReadByteThrow(headerCrc);
					}
					while(temp != 0);
				}

				// Skip the "comment" field.
				if((flags & GZipConstants.FCOMMENT) != 0)
				{
					do
					{
						temp = ReadByteThrow(headerCrc);
					}
					while(temp != 0);
				}

				// Read the header checksum.
				if((flags & GZipConstants.FHCRC) != 0)
				{
					magic1 = stream.ReadByte();
					if(magic1 < 0)
					{
						throw new IOException(S._("IO_Decompress_GzipHeader"));
					}
					temp = stream.ReadByte();
					if(temp < 0)
					{
						throw new IOException(S._("IO_Decompress_GzipHeader"));
					}
					temp = magic1 + (temp << 8);
					if(temp != (((int)(headerCrc.Value)) & 0xFFFF))
					{
						throw new IOException(S._("IO_Decompress_GzipHeader"));
					}
				}

				// The header has been read, and we are ready to go.
				headerDone = true;
			}

	// Read the gzip footer from the base stream.
	private void ReadFooter()
			{
				int temp;

				// Fetch 8 bytes from the input stream for the footer.
				// We may need to reclaim them from the inflater.
				byte[] footer = new byte [8];
				int size = inflater.RemainingInput;
				if(size > 8)
				{
					size = 8;
				}
				while(size < 8)
				{
					temp = stream.ReadByte();
					if(temp < 0)
					{
						throw new IOException(S._("IO_Decompress_GzipFooter"));
					}
					footer[size++] = (byte)temp;
				}

				// Validate the checksum.
				temp = footer[0] |
					   (footer[1] << 8) |
					   (footer[2] << 16) |
					   (footer[3] << 24);
				if(temp != (int)(crc32.Value))
				{
					throw new IOException(S._("IO_Decompress_Checksum"));
				}

				// Validate the total byte counter.
				temp = footer[4] |
					   (footer[5] << 8) |
					   (footer[6] << 16) |
					   (footer[7] << 24);
				if(temp != inflater.TotalOut)
				{
					throw new IOException(S._("IO_Decompress_Total"));
				}

				// We have now reached the end of the stream.
				endOfStream = true;
			}

	// Read data from this stream.
	public override int Read(byte[] buffer, int offset, int count)
			{
				int temp;
				if(stream == null)
				{
					throw new ObjectDisposedException
						(S._("Exception_Disposed"));
				}
				if(mode != CompressionMode.Decompress)
				{
					throw new NotSupportedException(S._("IO_NotSupp_Read"));
				}
				DeflateStream.ValidateBuffer(buffer, offset, count);
				if(!headerDone)
				{
					ReadHeader();
				}
				if(endOfStream)
				{
					return 0;
				}
				for(;;)
				{
					temp = inflater.Inflate(buffer, offset, count);
					if(temp > 0)
					{
						crc32.Update(buffer, offset, temp);
						if(inflater.IsFinished)
						{
							ReadFooter();
						}
						return temp;
					}
					if(inflater.IsNeedingDictionary)
					{
						throw new IOException
							(S._("IO_Decompress_NeedDict"));
					}
					else if(inflater.IsFinished)
					{
						ReadFooter();
						return 0;
					}
					else if(inflater.IsNeedingInput)
					{
						temp = stream.Read(buf, 0, buf.Length);
						if(temp <= 0)
						{
							throw new IOException
								(S._("IO_Decompress_Truncated"));
						}
						inflater.SetInput(buf, 0, temp);
					}
					else
					{
						throw new IOException
							(S._("IO_Decompress_Invalid"));
					}
				}
			}

	// Seek to a new position within this stream.
	public override long Seek(long offset, SeekOrigin origin)
			{
				throw new NotSupportedException(S._("IO_NotSupp_Seek"));
			}

	// Set the length of this stream.
	public override void SetLength(long value)
			{
				throw new NotSupportedException(S._("IO_NotSupp_SetLength"));
			}

	// Write data to this stream.
	public override void Write(byte[] buffer, int offset, int count)
			{
				int temp;
				if(stream == null)
				{
					throw new ObjectDisposedException
						(S._("Exception_Disposed"));
				}
				if(mode != CompressionMode.Compress)
				{
					throw new NotSupportedException(S._("IO_NotSupp_Write"));
				}
				DeflateStream.ValidateBuffer(buffer, offset, count);
				if(!headerDone)
				{
					// Write the gzip header to the output stream.
					stream.WriteByte(0x1F);		// Magic number.
					stream.WriteByte(0x8B);
					stream.WriteByte(0x08);		// Compression type.
					stream.WriteByte(0x00);		// Flags.
					int secs = (int)((DateTime.UtcNow.Ticks /
									  TimeSpan.TicksPerSecond) -
									  		62135596800L);
					stream.WriteByte((byte)(secs & 0xFF));
					stream.WriteByte((byte)((secs >> 8) & 0xFF));
					stream.WriteByte((byte)((secs >> 16) & 0xFF));
					stream.WriteByte((byte)((secs >> 24) & 0xFF));
					stream.WriteByte(0x00);		// Extra flags.
					stream.WriteByte(0xFF);		// OS type (unknown).
					headerDone = true;
				}
				crc32.Update(buffer, offset, count);
				deflater.SetInput(buffer, offset, count);
				while(!deflater.IsNeedingInput)
				{
					temp = deflater.Deflate(buf, 0, buf.Length);
					if(temp <= 0)
					{
						if(!deflater.IsNeedingInput)
						{
							throw new IOException(S._("IO_Compress_Input"));
						}
						break;
					}
					stream.Write(buf, 0, temp);
				}
			}

}; // class GZipStream

#endif // CONFIG_COMPRESSION

}; // namespace System.IO.Compression
