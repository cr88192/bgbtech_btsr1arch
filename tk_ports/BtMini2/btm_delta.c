/*
Tag Structure:
  FD (Tag:BYTE) (Size:DWORD) (Data:BYTE[Size-6])
  FE (Tag:BYTE) (Size:WORD) (Data:BYTE[Size-4])
    Top Level Message.
    Tag Byte gives major message type.
  FF: Pad Byte
  
Tag Bytes:
  'C': Chunk Delta
  'D': Delta
  'E': Entity Delta (ABXE)
  
Chunk Delta:
  VerSeq:DWORD
  ChunkPos:DWORD
    (11: 0): Chunk X
    (23:12): Chunk Y
    (26:24): Chunk Z
    (31:27): Dimension
  Chunk Data Follows
    Will folow the same format as in the region file.
    Starts with a Header DWORD

...

 */

int BTM_ParseDelta(BTM_World *wrl, byte *msgbuf, int msgsz)
{
}
