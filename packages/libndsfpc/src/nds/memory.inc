{$ifdef NDS_INTERFACE}

const
{$ifdef ARM9}
  REG_EXMEMCNT : pcuint16 = pointer($04000204);
{$else}
  REG_EXMEMSTAT: pcuint16 = pointer($04000204);
{$endif}
  ARM7_MAIN_RAM_PRIORITY  = (1 shl 15);
  ARM7_OWNS_CARD          = (1 shl 11);
  ARM7_OWNS_ROM           = (1 shl 7);


  REG_MBK1: pcuint8 = pointer($04004040); // WRAM_A 0..3 
  REG_MBK2: pcuint8 = pointer($04004044); // WRAM_B 0..3 
  REG_MBK3: pcuint8 = pointer($04004048); // WRAM_B 4..7 
  REG_MBK4: pcuint8 = pointer($0400404C); // WRAM_C 0..3 
  REG_MBK5: pcuint8 = pointer($04004050); // WRAM_C 4..7 
  REG_MBK6: pcuint32 = pointer($04004054);
  REG_MBK7: pcuint32 = pointer($04004058);
  REG_MBK8: pcuint32 = pointer($0400405C);
  REG_MBK9: pcuint32 = pointer($04004060);

// Protection register (write-once sadly)
{$ifdef ARM7}
  PROTECTION    : pcuint32 = pointer($04000308);
{$endif ARM7}


  ALLRAM        : pcuint8  = pointer($00000000);
  
  MAINRAM8      : pcuint8  = pointer($02000000);
  MAINRAM16     : pcuint16 = pointer($02000000);
  MAINRAM32     : pcuint32 = pointer($02000000);

// fixme: shared RAM

// GBA_BUS is volatile, while GBAROM is not
  GBA_BUS       : pcuint16 = pointer($08000000);
  GBAROM        : pcuint16 = pointer($08000000);

  SRAM          : pcuint8  = pointer($0A000000);

{$ifdef ARM7}
  VRAM          : pcuint16 = pointer($06000000);
{$endif ARM7}

type
  sGBAHeader = packed record
    entryPoint: cuint32;
    logo: array [0..155] of cuint8;
    title: array [0..11] of cchar;
    gamecode: array [0..3] of cchar;
    makercode: cuint16;
    is96h: cuint8;
    unitcode: cuint8;
    devicecode: cuint8;
    unused: array [0..6] of cuint8;
    version: cuint8;
    complement: cuint8;
    checksum: cuint16;
  end; 
  TGBAHeader = sGBAHeader;
  PGBAHeader = ^sGBAHeader;

const
  GBA_HEADER : pGBAHeader = pointer($08000000);

type
  sNDSHeader = packed record
    gameTitle: array [0..11] of cchar;
    gameCode: array [0..3] of cchar;
    makercode: array [0..1] of cchar;
    unitCode: cuint8;
    deviceType: cuint8;           // type of device in the game card
    deviceSize: cuint8;           // device capacity (1<<n Mbit)
    reserved1: array [0..8] of cuint8;
    romversion: cuint8;
    flags: cuint8;                // auto-boot flag
    
    arm9romOffset: cuint32;
    arm9executeAddress: pointer;
    arm9destination: pointer;
    arm9binarySize: cuint32;
    
    arm7romOffset: cuint32;
    arm7executeAddress: pointer;
    arm7destination: pointer;
    arm7binarySize: cuint32;
    
    filenameOffset: cuint32;
    filenameSize: cuint32;
    fatOffset: cuint32;
    fatSize: cuint32;
    
    arm9overlaySource: cuint32;
    arm9overlaySize: cuint32;
    arm7overlaySource: cuint32;
    arm7overlaySize: cuint32;
    
    cardControl13: cuint32;  // used in modes 1 and 3
    cardControlBF: cuint32;  // used in mode 2
    bannerOffset: cuint32;
    
    secureCRC16: cuint16;
    
    readTimeout: cuint16;
    
    unknownRAM1: cuint32;
    unknownRAM2: cuint32;
    
    bfPrime1: cuint32;
    bfPrime2: cuint32;
    romSize: cuint32;
    
    headerSize: cuint32;
    zeros88: array [0..13] of cuint32;
    gbaLogo: array [0..155] of cuint8;
    logoCRC16: cuint16;
    headerCRC16: cuint16;
  end;
  tNDSHeader = sNDSHeader;
  pNDSHeader = ^tNDSHeader;
  
  DSiHeader = packed record  
    ndshdr: tNDSHeader;  
    debugRomSource: cuint32;
    debugRomSize: cuint32;
    debugRomDestination: cuint32;
    offset_0x16C: cuint32;
    
    zero: array [0..15] of cuint8;
      
   	global_mbk_setting: array [0..4, 0..3] of cuint8;
  	arm9_mbk_setting: array [0..2] of cuint32;
  	arm7_mbk_setting: array [0..2] of cuint32;
  	mbk9_wramcnt_setting: cuint32;
  
  	region_flags: cuint32;
  	access_control: cuint32;
  	scfg_ext_mask: cuint32;
  	offset_0x1BC: array [0..2] of cuint8;
  	appflags: cuint8;
  
  	arm9iromOffset: pointer;
  	offset_0x1C4: cuint32;
  	arm9idestination: pointer;
  	arm9ibinarySize: cuint32;
  	arm7iromOffset: pointer;
  	offset_0x1D4: cuint32;
  	arm7idestination: pointer;
  	arm7ibinarySize: cuint32;
  
  	digest_ntr_start: cuint32;
  	digest_ntr_size: cuint32;
  	digest_twl_start: cuint32;
  	digest_twl_size: cuint32;
  	sector_hashtable_start: cuint32;
  	sector_hashtable_size: cuint32;
  	block_hashtable_start: cuint32;
  	block_hashtable_size: cuint32;
  	digest_sector_size: cuint32;
  	digest_block_sectorcount: cuint32;
  
  	banner_size: cuint32;
  	offset_0x20C: cuint32;
  	total_rom_size: cuint32;
  	offset_0x214: cuint32;
  	offset_0x218: cuint32;
  	offset_0x21C: cuint32;
  
  	modcrypt1_start: cuint32;
  	modcrypt1_size: cuint32;
  	modcrypt2_start: cuint32;
  	modcrypt2_size: cuint32;
  
  	tid_low: cuint32;
  	tid_high: cuint32;
  	public_sav_size: cuint32;
  	private_sav_size: cuint32;
  	reserved3: array [0..175] of cuint8;
  	age_ratings: array [0..15] of cuint8;
  
  	hmac_arm9: array [0..19] of cuint8;
  	hmac_arm7: array [0..19] of cuint8;
  	hmac_digest_master: array [0..19] of cuint8;
  	hmac_icon_title: array [0..19] of cuint8;
  	hmac_arm9i: array [0..19] of cuint8;
  	hmac_arm7i: array [0..19] of cuint8;
  	reserved4: array [0..39] of cuint8;
  	hmac_arm9_no_secure: array [0..19] of cuint8;
  	reserved5: array [0..2635] of cuint8;
  	debug_args: array [0..383] of cuint8;
  	rsa_signature: array [0..127] of cuint8;  
  end;
  T__DSiHeader = DSiHeader;
  P__DSiHeader = ^T__DSiHeader;
  
  
const
  __NDSHeader : pNDSHeader   = pointer($02FFFE00);
  __DSiHeader : P__DSiHeader = pointer($02FFE000);

type
  sNDSBanner = packed record
    version: cuint16;
    crc: cuint16;
    reserved: array [0..27] of cuint8;
    icon: array [0..511] of cuint8;
    palette: array [0..15] of cuint16;
    titles: array [0..5, 0..127] of cuint16;
  end;
  tNDSBanner = sNDSBanner;
  pNDSBanner = ^tNDSBanner;
{$endif NDS_INTERFACE}


{$ifdef ARM9}
{$ifdef NDS_INTERFACE}
const
  BUS_OWNER_ARM9 = true;
  BUS_OWNER_ARM7 = false;
  
procedure sysSetCartOwner(arm9: cbool); inline; 
procedure sysSetCardOwner(arm9: cbool); inline;
procedure sysSetBusOwners(arm9rom, arm9card: cbool); inline;
{$endif NDS_INTERFACE}

{$ifdef NDS_IMPLEMENTATION}
// Changes only the gba rom bus ownership
procedure sysSetCartOwner(arm9: cbool); inline; 
var
  i: cint;
begin
  if arm9 then
    i := 0
  else
    i := ARM7_OWNS_ROM;
  REG_EXMEMCNT^ := (REG_EXMEMCNT^ and not ARM7_OWNS_ROM) or (i);
end;

// Changes only the nds card bus ownership
procedure sysSetCardOwner(arm9: cbool); inline;
var
  i: cint;
begin
  if arm9 then
    i := 0
  else
    i := ARM7_OWNS_ROM;
  REG_EXMEMCNT^ := (REG_EXMEMCNT^ and not ARM7_OWNS_CARD) or (i);
end;

// Changes all bus ownerships
procedure sysSetBusOwners(arm9rom, arm9card: cbool); inline;
var
  val1, val2: cuint;
begin
  val1 := 0;
  val2 := 0;
  if not arm9card then
    val1 := ARM7_OWNS_CARD; 
  
  if not arm9rom then
    val2 := ARM7_OWNS_ROM ;
    
  REG_EXMEMCNT^ := (REG_EXMEMCNT^ and not(ARM7_OWNS_CARD or ARM7_OWNS_ROM)) or val1 or val2;
end;


{$endif NDS_IMPLEMENTATION}
{$endif ARM9}
