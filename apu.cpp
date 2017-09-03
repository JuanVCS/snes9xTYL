/**********************************************************************************
  Snes9x - Portable Super Nintendo Entertainment System (TM) emulator.

  (c) Copyright 1996 - 2002  Gary Henderson (gary.henderson@ntlworld.com),
                             Jerremy Koot (jkoot@snes9x.com)

  (c) Copyright 2002 - 2004  Matthew Kendora

  (c) Copyright 2002 - 2005  Peter Bortas (peter@bortas.org)

  (c) Copyright 2004 - 2005  Joel Yliluoma (http://iki.fi/bisqwit/)

  (c) Copyright 2001 - 2006  John Weidman (jweidman@slip.net)

  (c) Copyright 2002 - 2006  funkyass (funkyass@spam.shaw.ca),
                             Kris Bleakley (codeviolation@hotmail.com)

  (c) Copyright 2002 - 2007  Brad Jorsch (anomie@users.sourceforge.net),
                             Nach (n-a-c-h@users.sourceforge.net),
                             zones (kasumitokoduck@yahoo.com)

  (c) Copyright 2006 - 2007  nitsuja


  BS-X C emulator code
  (c) Copyright 2005 - 2006  Dreamer Nom,
                             zones

  C4 x86 assembler and some C emulation code
  (c) Copyright 2000 - 2003  _Demo_ (_demo_@zsnes.com),
                             Nach,
                             zsKnight (zsknight@zsnes.com)

  C4 C++ code
  (c) Copyright 2003 - 2006  Brad Jorsch,
                             Nach

  DSP-1 emulator code
  (c) Copyright 1998 - 2006  _Demo_,
                             Andreas Naive (andreasnaive@gmail.com)
                             Gary Henderson,
                             Ivar (ivar@snes9x.com),
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora,
                             Nach,
                             neviksti (neviksti@hotmail.com)

  DSP-2 emulator code
  (c) Copyright 2003         John Weidman,
                             Kris Bleakley,
                             Lord Nightmare (lord_nightmare@users.sourceforge.net),
                             Matthew Kendora,
                             neviksti


  DSP-3 emulator code
  (c) Copyright 2003 - 2006  John Weidman,
                             Kris Bleakley,
                             Lancer,
                             z80 gaiden

  DSP-4 emulator code
  (c) Copyright 2004 - 2006  Dreamer Nom,
                             John Weidman,
                             Kris Bleakley,
                             Nach,
                             z80 gaiden

  OBC1 emulator code
  (c) Copyright 2001 - 2004  zsKnight,
                             pagefault (pagefault@zsnes.com),
                             Kris Bleakley,
                             Ported from x86 assembler to C by sanmaiwashi

  SPC7110 and RTC C++ emulator code
  (c) Copyright 2002         Matthew Kendora with research by
                             zsKnight,
                             John Weidman,
                             Dark Force

  S-DD1 C emulator code
  (c) Copyright 2003         Brad Jorsch with research by
                             Andreas Naive,
                             John Weidman

  S-RTC C emulator code
  (c) Copyright 2001-2006    byuu,
                             John Weidman

  ST010 C++ emulator code
  (c) Copyright 2003         Feather,
                             John Weidman,
                             Kris Bleakley,
                             Matthew Kendora

  Super FX x86 assembler emulator code
  (c) Copyright 1998 - 2003  _Demo_,
                             pagefault,
                             zsKnight,

  Super FX C emulator code
  (c) Copyright 1997 - 1999  Ivar,
                             Gary Henderson,
                             John Weidman

  Sound DSP emulator code is derived from SNEeSe and OpenSPC:
  (c) Copyright 1998 - 2003  Brad Martin
  (c) Copyright 1998 - 2006  Charles Bilyue'

  SH assembler code partly based on x86 assembler code
  (c) Copyright 2002 - 2004  Marcus Comstedt (marcus@mc.pp.se)

  2xSaI filter
  (c) Copyright 1999 - 2001  Derek Liauw Kie Fa

  HQ2x, HQ3x, HQ4x filters
  (c) Copyright 2003         Maxim Stepin (maxim@hiend3d.com)

  Win32 GUI code
  (c) Copyright 2003 - 2006  blip,
                             funkyass,
                             Matthew Kendora,
                             Nach,
                             nitsuja

  Mac OS GUI code
  (c) Copyright 1998 - 2001  John Stiles
  (c) Copyright 2001 - 2007  zones


  Specific ports contains the works of other authors. See headers in
  individual files.


  Snes9x homepage: http://www.snes9x.com

  Permission to use, copy, modify and/or distribute Snes9x in both binary
  and source form, for non-commercial purposes, is hereby granted without
  fee, providing that this license information and copyright notice appear
  with all copies and any derived work.

  This software is provided 'as-is', without any express or implied
  warranty. In no event shall the authors be held liable for any damages
  arising from the use of this software or it's derivatives.

  Snes9x is freeware for PERSONAL USE only. Commercial users should
  seek permission of the copyright holders first. Commercial use includes,
  but is not limited to, charging money for Snes9x or software derived from
  Snes9x, including Snes9x or derivatives in commercial game bundles, and/or
  using Snes9x as a promotion for your commercial product.

  The copyright holders request that bug fixes and improvements to the code
  should be forwarded to them so everyone can benefit from the modifications
  in future versions.

  Super NES and Super Nintendo Entertainment System are trademarks of
  Nintendo Co., Limited and its subsidiary companies.
**********************************************************************************/

#include "port.h"

#include "snes9x.h"
#include "spc700.h"
#include "apu.h"
#include "soundux.h"
//#include "cpuexec.h"

extern int32 env_counter_table[32];
extern int NoiseFreq [32];

#ifdef ME_SOUND
SAPUEVENTS __attribute__((aligned(64))) stAPUEvents2;
SAPUEVENTS  *  const __attribute__((aligned(64))) stAPUEvents_p=(SAPUEVENTS*)UNCACHE_PTR(&stAPUEvents);
//SAPUEVENTS  *  const __attribute__((aligned(64))) stAPUEvents_p=(SAPUEVENTS*)UNCACHE_PTR(0x10000+sizeof (struct SCPUPACK));//�ł��Ȃ�

int32 __attribute__((aligned(64))) Uncache_APU_Cycles_value;
//volatile int32 __attribute__((aligned(64))) *   const Uncache_APU_Cycles_p=(int*)UNCACHE_PTR(&Uncache_APU_Cycles_value);
//volatile int32 *  const __attribute__((aligned(64))) Uncache_APU_Cycles_p=(int*)UNCACHE_PTR(&(APUuncached.Cycles));

bool8 __attribute__((aligned(64))) Uncache_IAPU_APUExecuting;

int32 Uncache_APU_OutPorts_value;\
//uint8 * const Uncache_APU_OutPorts_p=(uint8*)UNCACHE_PTR(&Uncache_APU_OutPorts_value);
//uint8 * const Uncache_APU_OutPorts_p=(uint8*)UNCACHE_PTR(APUuncached.OutPorts);

#ifdef FAST_IAPU_APUEXECUTING_CHECK
//bool8 IAPU_APUExecuting_Main;
//int apu_glob_cycles_Main;
//uint8 APUExecuting_Main_Counter;
struct SIAPU_APUExecuting _IAPU_APUExecuting;
#endif

//bool8 __attribute__((aligned(64))) apu_vars[4*10+65536*4*2+65536*2];

//#undef IAPU
//SIAPU* Uncache_IAPU_p=(SIAPU*)(&APUPack.IAPU);
//#define IAPU (*Uncache_IAPU_p)
#endif
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void S9xDeinitAPU () {
	/*if ((IAPUuncached.RAM)) {
		free ((char *) NORMAL_PTR(IAPUuncached.RAM));
		(IAPUuncached.RAM) = NULL;
  }
  if ((IAPUuncached)) {
		free ((char *) NORMAL_PTR(IAPUuncached));
		(IAPUuncached) = NULL;
  }
  if ((APUuncached)) {
		free ((char *) NORMAL_PTR(APUuncached));
		(APUuncached) = NULL;
  }
  if ((APURegistersUncached)) {
		free ((char *) NORMAL_PTR(APURegistersUncached));
		(APURegistersUncached) = NULL;
  }
  
  if ((apu_cycles_left)) {
		free ((char *) NORMAL_PTR(apu_cycles_left));
		(apu_cycles_left) = NULL;
  }*/

	
}

EXTERN_C uint8 APUROM [64];
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void S9xResetAPU ()
{

    int i;
	S9xSuspendSoundProcess();

    Settings.APUEnabled = Settings.NextAPUEnabled;
    
    //taken from sneese
    APUI00a=APUI00b=APUI00c=0;
    APUI01a=APUI01b=APUI01c=0;
    APUI02a=APUI02b=APUI02c=0;
    APUI03a=APUI03b=APUI03c=0;
    
    apu_init_after_load=0;    
    //apu_can_execute=0;
    
	if(Settings.APUEnabled)
		APUPack.APU.Flags &= ~HALTED_FLAG;
        
	//memset((IAPUuncached.RAM),0, 0x10000);
	ZeroMemory(IAPUuncached.RAM, 0x100);
	memset((IAPUuncached.RAM)+0x20, 0xFF, 0x20);
	memset((IAPUuncached.RAM)+0x60, 0xFF, 0x20);
	memset((IAPUuncached.RAM)+0xA0, 0xFF, 0x20);
	memset((IAPUuncached.RAM)+0xE0, 0xFF, 0x20);

	for(i=1;i<256;i++)
	{
		memcpy((IAPUuncached.RAM)+(i<<8), (IAPUuncached.RAM), 0x100);
	}

    ZeroMemory ((APUPack.APU.OutPorts), 4);   
    ZeroMemory ((void*)Uncache_APU_OutPorts, 4);   
    memmove (&(IAPUuncached.RAM) [0xffc0], APUROM, sizeof (APUROM));
    memmove ((APUuncached.ExtraRAM), &(IAPUuncached.RAM)[0xffc0], sizeof (APUROM));
    (IAPUuncached.DirectPage) = (IAPUuncached.RAM);
    (IAPUuncached.PC) = (IAPUuncached.RAM) + (IAPUuncached.RAM) [0xfffe] + ((IAPUuncached.RAM) [0xffff] << 8);
    APUPack.APU.Cycles = 0;
	Uncache_APU_Cycles = 0;
    apu_cycles_left=0;

//sprintf (tmp,"ResetApu1 %d,%d",apu_event1_cpt1,apu_glob_cycles);//msgBoxLines(tmp,80);
    apu_event1_cpt1=0;
	apu_event1_cpt2=0;
    apu_event2_cpt1=0;
    apu_event2_cpt2=0;
    apu_ram_write_cpt1=0;
    apu_ram_write_cpt2=0;
#ifdef ME_SOUND
	apu_ram_write_cpt2_main=0;
	apu_event1_cpt2_main=0;
#endif
  
    (APURegistersUncached.YA).W = 0;
    (APURegistersUncached.X) = 0;
    (APURegistersUncached.S) = 0xef;
    (APURegistersUncached.P) = 0x02;
    S9xAPUUnpackStatusUncached ();
    (APURegistersUncached.PC) = 0;
    (IAPU_APUExecuting) = Settings.APUEnabled;
#ifdef SPC700_SHUTDOWN
    (IAPUuncached.WaitAddress1) = NULL;
    (IAPUuncached.WaitAddress2) = NULL;
    (IAPUuncached.WaitCounter) = 0;
#endif
	//(IAPUuncached.NextAPUTimerPos) = 0;
//	(IAPUuncached.APUTimerCounter) = 0;
    (APUuncached.ShowROM) = TRUE;
    (IAPUuncached.RAM) [0xf1] = 0x80;
		
	IAPUuncached.Address=0;
    for (i = 0; i < 3; i++)
    {
		(APUuncached.TimerEnabled) [i] = FALSE;
		(APUuncached.TimerValueWritten) [i] = 0;
		(APUuncached.TimerTarget) [i] = 0;
		(APUuncached.Timer) [i] = 0;
    }
    for (int j = 0; j < 0x80; j++)
		(APUuncached.DSP) [j] = 0;
	
    (IAPUuncached.TwoCycles) = (IAPUuncached.OneCycle) * 2;
	    	
    (APUuncached.DSP) [APU_ENDX] = 0;
    (APUuncached.DSP) [APU_KOFF] = 0;
    (APUuncached.DSP) [APU_KON] = 0;
    (APUuncached.DSP) [APU_FLG] = APU_MUTE | APU_ECHO_DISABLED;
    (APUuncached.KeyedChannels) = 0;



#ifdef ME_SOUND
	for (i = 0; i < 256; i++)
		S9xAPUCycles [i] = S9xAPUCycleLengths [i] * (int)(IAPU.OneCycle) *os9x_apu_ratio / 256;

	S9xResetSound (TRUE);
	S9xSetEchoEnable (0);
				//apu_init_after_load|=1;
#else
	apu_init_after_load|=1;
#endif

	 S9xResumeSoundProcess();
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void S9xSetAPUDSP (uint8 byte)
{
	static uint8 KeyOn;
	static uint8 KeyOnPrev;
	int pitch;

	uint8 reg = (IAPU.RAM) [0xf2];	
  
    switch (reg)
    {
    case APU_FLG:
		if (byte & APU_SOFT_RESET)
		{
			APUPack.APU.DSP[reg] = APU_MUTE | APU_ECHO_DISABLED | (byte & 0x1f);
			APUPack.APU.DSP[APU_ENDX] = 0;
			APUPack.APU.DSP[APU_KOFF] = 0;
			APUPack.APU.DSP[APU_KON] = 0;
			S9xSetEchoWriteEnable (FALSE);
			// Kill sound
			S9xResetSound (FALSE);
		}
		else
		{
			S9xSetEchoWriteEnable (!(byte & APU_ECHO_DISABLED));
			if (byte & APU_MUTE)
			{
				S9xSetSoundMute (TRUE);
			}
			else
				S9xSetSoundMute (FALSE);
			
			SoundData.noise_rate = env_counter_table[byte & 0x1f];
		}
		break;
    case APU_NON:
		if (byte != APUPack.APU.DSP[APU_NON])
		{
			uint8 mask = 1;
			for (int c = 0; c < 8; c++, mask <<= 1)
			{
				int type;
				if (byte & mask)
				{
					type = SOUND_NOISE;
				}
				else
				{
					type = SOUND_SAMPLE;
				}
				S9xSetSoundType (c, type);
			}
		}
		break;
    case APU_MVOL_LEFT:
		if (byte != APUPack.APU.DSP[APU_MVOL_LEFT])
		{
			S9xSetMasterVolume ((signed char) byte,
				(signed char) APUPack.APU.DSP[APU_MVOL_RIGHT]);
		}
		break;
    case APU_MVOL_RIGHT:
		if (byte != APUPack.APU.DSP[APU_MVOL_RIGHT])
		{
			S9xSetMasterVolume ((signed char) APUPack.APU.DSP[APU_MVOL_LEFT],
				(signed char) byte);
		}
		break;
    case APU_EVOL_LEFT:
		if (byte != APUPack.APU.DSP[APU_EVOL_LEFT])
		{
			S9xSetEchoVolume ((signed char) byte,
				(signed char) APUPack.APU.DSP[APU_EVOL_RIGHT]);
		}
		break;
    case APU_EVOL_RIGHT:
		if (byte != APUPack.APU.DSP[APU_EVOL_RIGHT])
		{
			S9xSetEchoVolume ((signed char) APUPack.APU.DSP[APU_EVOL_LEFT],
				(signed char) byte);
		}
		break;
    case APU_ENDX:
		byte = 0;
		break;
		
    case APU_KOFF:
		//		if (byte)
		{
			uint8 mask = 1;
			for (int c = 0; c < 8; c++, mask <<= 1)
			{
				if ((byte & mask) != 0)
				{
					if ((APUPack.APU.KeyedChannels) & mask)
					{
						{
							KeyOnPrev&=~mask;
							(APUPack.APU.KeyedChannels) &= ~mask;
							APUPack.APU.DSP[APU_KON] &= ~mask;
							//APUPack.APU.DSP[APU_KOFF] |= mask;
							S9xSetSoundKeyOff (c);
						}
					}
				}
				else if((KeyOnPrev&mask)!=0)
				{
					KeyOnPrev&=~mask;
					(APUPack.APU.KeyedChannels) |= mask;
					//APUPack.APU.DSP[APU_KON] |= mask;
					APUPack.APU.DSP[APU_KOFF] &= ~mask;
					APUPack.APU.DSP[APU_ENDX] &= ~mask;
					S9xPlaySample (c);
				}
			}
		}
		//KeyOnPrev=0;
		APUPack.APU.DSP[APU_KOFF] = byte;
		return;
    case APU_KON:
		if (byte)
		{
			uint8 mask = 1;
			for (int c = 0; c < 8; c++, mask <<= 1)
			{
				if ((byte & mask) != 0)
				{
					// Pac-In-Time requires that channels can be key-on
					// regardeless of their current state.
					if((APUPack.APU.DSP[APU_KOFF] & mask) ==0)
					{
						KeyOnPrev&=~mask;
						(APUPack.APU.KeyedChannels) |= mask;
						//APUPack.APU.DSP[APU_KON] |= mask;
						//APUPack.APU.DSP[APU_KOFF] &= ~mask;
						APUPack.APU.DSP[APU_ENDX] &= ~mask;
						S9xPlaySample (c);
					}
					else KeyOn|=mask;
				}
			}
		}
		return;
		
    case APU_VOL_LEFT + 0x00:
    case APU_VOL_LEFT + 0x10:
    case APU_VOL_LEFT + 0x20:
    case APU_VOL_LEFT + 0x30:
    case APU_VOL_LEFT + 0x40:
    case APU_VOL_LEFT + 0x50:
    case APU_VOL_LEFT + 0x60:
    case APU_VOL_LEFT + 0x70:
		// At Shin Megami Tensei suggestion 6/11/00
		//	if (byte != APUPack.APU.DSP[reg])
		S9xSetSoundVolume (reg >> 4, (signed char) byte, (signed char) APUPack.APU.DSP[reg + 1]);
		break;
    case APU_VOL_RIGHT + 0x00:
    case APU_VOL_RIGHT + 0x10:
    case APU_VOL_RIGHT + 0x20:
    case APU_VOL_RIGHT + 0x30:
    case APU_VOL_RIGHT + 0x40:
    case APU_VOL_RIGHT + 0x50:
    case APU_VOL_RIGHT + 0x60:
    case APU_VOL_RIGHT + 0x70:
		// At Shin Megami Tensei suggestion 6/11/00
		//	if (byte != APUPack.APU.DSP[reg])
		S9xSetSoundVolume (reg >> 4, (signed char) APUPack.APU.DSP[reg - 1], (signed char) byte);
		break;
		
    case APU_P_LOW + 0x00:
    case APU_P_LOW + 0x10:
    case APU_P_LOW + 0x20:
    case APU_P_LOW + 0x30:
    case APU_P_LOW + 0x40:
    case APU_P_LOW + 0x50:
    case APU_P_LOW + 0x60:
    case APU_P_LOW + 0x70:
		pitch = (((int)byte + ((int)APUPack.APU.DSP [reg + 1] << 8)) & FREQUENCY_MASK);
		S9xSetSoundHertz (reg >> 4, pitch * 8);
		//S9xSetSoundHertz (reg >> 4, ((byte + (APUPack.APU.DSP [reg + 1] << 8)) & FREQUENCY_MASK) * 8);
		break;
		
    case APU_P_HIGH + 0x00:
    case APU_P_HIGH + 0x10:
    case APU_P_HIGH + 0x20:
    case APU_P_HIGH + 0x30:
    case APU_P_HIGH + 0x40:
    case APU_P_HIGH + 0x50:
    case APU_P_HIGH + 0x60:
    case APU_P_HIGH + 0x70:
		pitch = ((((int)byte << 8) + (int)APUPack.APU.DSP [reg - 1]) & FREQUENCY_MASK);
		S9xSetSoundHertz (reg >> 4, pitch * 8);
		//S9xSetSoundHertz (reg >> 4, (((byte << 8) + APUPack.APU.DSP [reg - 1]) & FREQUENCY_MASK) * 8);
		break;
		
    case APU_SRCN + 0x00:
    case APU_SRCN + 0x10:
    case APU_SRCN + 0x20:
    case APU_SRCN + 0x30:
    case APU_SRCN + 0x40:
    case APU_SRCN + 0x50:
    case APU_SRCN + 0x60:
    case APU_SRCN + 0x70:
		break;
		
    case APU_ADSR1 + 0x00:
    case APU_ADSR1 + 0x10:
    case APU_ADSR1 + 0x20:
    case APU_ADSR1 + 0x30:
    case APU_ADSR1 + 0x40:
    case APU_ADSR1 + 0x50:
    case APU_ADSR1 + 0x60:
    case APU_ADSR1 + 0x70:
		if (byte != APUPack.APU.DSP[reg])
			S9xFixEnvelope (reg >> 4, APUPack.APU.DSP[reg + 2], byte, APUPack.APU.DSP[reg + 1]);
		break;
		
    case APU_ADSR2 + 0x00:
    case APU_ADSR2 + 0x10:
    case APU_ADSR2 + 0x20:
    case APU_ADSR2 + 0x30:
    case APU_ADSR2 + 0x40:
    case APU_ADSR2 + 0x50:
    case APU_ADSR2 + 0x60:
    case APU_ADSR2 + 0x70:
		if (byte != APUPack.APU.DSP[reg])
			S9xFixEnvelope (reg >> 4, APUPack.APU.DSP[reg + 1], APUPack.APU.DSP[reg - 1], byte);
		break;
		
    case APU_GAIN + 0x00:
    case APU_GAIN + 0x10:
    case APU_GAIN + 0x20:
    case APU_GAIN + 0x30:
    case APU_GAIN + 0x40:
    case APU_GAIN + 0x50:
    case APU_GAIN + 0x60:
    case APU_GAIN + 0x70:
		if (byte != APUPack.APU.DSP[reg])
			S9xFixEnvelope (reg >> 4, byte, APUPack.APU.DSP[reg - 2], APUPack.APU.DSP[reg - 1]);
		break;
		
    case APU_ENVX + 0x00:
    case APU_ENVX + 0x10:
    case APU_ENVX + 0x20:
    case APU_ENVX + 0x30:
    case APU_ENVX + 0x40:
    case APU_ENVX + 0x50:
    case APU_ENVX + 0x60:
    case APU_ENVX + 0x70:
		break;
		
    case APU_OUTX + 0x00:
    case APU_OUTX + 0x10:
    case APU_OUTX + 0x20:
    case APU_OUTX + 0x30:
    case APU_OUTX + 0x40:
    case APU_OUTX + 0x50:
    case APU_OUTX + 0x60:
    case APU_OUTX + 0x70:
		break;
		
    case APU_DIR:
		break;
		
    case APU_PMON:
		if (byte != APUPack.APU.DSP[APU_PMON])
			S9xSetFrequencyModulationEnable (byte);
		break;
		
    case APU_EON:
		if (byte != APUPack.APU.DSP[APU_EON])
			S9xSetEchoEnable (byte);
		break;
	
    case APU_EFB:
		S9xSetEchoFeedback ((signed char) byte);
		break;
		
    case APU_ESA:
		break;
		
    case APU_EDL:
		S9xSetEchoDelay (byte & 0xf);
		break;
		
    case APU_C0:
    case APU_C1:
    case APU_C2:
    case APU_C3:
    case APU_C4:
    case APU_C5:
    case APU_C6:
    case APU_C7:
		S9xSetFilterCoefficient (reg >> 4, (signed char) byte);
		break;
    default:
		// XXX
		//printf ("Write %02x to unknown APU register %02x\n", byte, reg);
		break;
    }
	
	KeyOnPrev|=KeyOn;
	KeyOn=0;
	
    if (reg < 0x80)
		APUPack.APU.DSP[reg] = byte;
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
// ADSR mode
void S9xFixEnvelope (int channel, uint8 gain, uint8 adsr1, uint8 adsr2)
{
    if (adsr1 & 0x80)
    {
		if (S9xSetSoundMode (channel, MODE_ADSR))
			S9xSetSoundADSR (channel, adsr1 & 0xf, (adsr1 >> 4) & 7,
				adsr2 & 0x1f, (adsr2 >> 5) & 7);
    }
    else
	{
		if ((gain & 0x80) == 0)
		{
			if (S9xSetSoundMode (channel, MODE_GAIN))
				S9xSetEnvelopeHeight (channel, (gain & 0x7f) << ENV_SHIFT);
		}
		else
		{
			if (gain & 0x40)
			{
				if (S9xSetSoundMode (channel, (gain & 0x20) ?
					MODE_INCREASE_BENT_LINE : MODE_INCREASE_LINEAR))
					S9xSetEnvelopeRate (channel, env_counter_table[gain & 0x1f], ENV_MAX);
			}
			else
			{
				if (S9xSetSoundMode (channel, (gain & 0x20) ?
					MODE_DECREASE_EXPONENTIAL : MODE_DECREASE_LINEAR))
					S9xSetEnvelopeRate (channel, env_counter_table[gain & 0x1f], 0);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void S9xSetAPUControl (uint8 byte)
{
//WAIT4MIXING()
	//if (byte & 0x40)
	//printf ("*** Special SPC700 timing enabled\n");
    if ((byte & 1) != 0 && !(APUPack.APU.TimerEnabled) [0])
    {
		(APUPack.APU.Timer) [0] = 0;
		(IAPU.RAM) [0xfd] = 0;
		if (((APUPack.APU.TimerTarget) [0] = (IAPU.RAM) [0xfa]) == 0)
			(APUPack.APU.TimerTarget) [0] = 0x100;
    }
    if ((byte & 2) != 0 && !(APUPack.APU.TimerEnabled) [1])
    {
		(APUPack.APU.Timer) [1] = 0;
		(IAPU.RAM) [0xfe] = 0;
		if (((APUPack.APU.TimerTarget) [1] = (IAPU.RAM) [0xfb]) == 0)
			(APUPack.APU.TimerTarget) [1] = 0x100;
    }
    if ((byte & 4) != 0 && !(APUPack.APU.TimerEnabled) [2])
    {
		(APUPack.APU.Timer) [2] = 0;
		(IAPU.RAM) [0xff] = 0;
		if (((APUPack.APU.TimerTarget) [2] = (IAPU.RAM) [0xfc]) == 0)
			(APUPack.APU.TimerTarget) [2] = 0x100;
    }
    (APUPack.APU.TimerEnabled) [0] = byte & 1;
    (APUPack.APU.TimerEnabled) [1] = (byte & 2) >> 1;
    (APUPack.APU.TimerEnabled) [2] = (byte & 4) >> 2;
	
    if (byte & 0x10)
		(IAPU.RAM) [0xF4] = (IAPU.RAM) [0xF5] = 0;
	
    if (byte & 0x20)
		(IAPU.RAM) [0xF6] = (IAPU.RAM) [0xF7] = 0;
	
    if (byte & 0x80)
    {
		if (!(APUPack.APU.ShowROM))
		{
			memcpy (&(IAPU.RAM) [0xffc0], APUROM, sizeof (APUROM));
			(APUPack.APU.ShowROM) = TRUE;
		}
    }
    else
    {
		if ((APUPack.APU.ShowROM))
		{
			(APUPack.APU.ShowROM) = FALSE;
			memcpy (&(IAPU.RAM) [0xffc0], (APUPack.APU.ExtraRAM), sizeof (APUROM));
		}
    }
    (IAPU.RAM) [0xf1] = byte;
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void S9xSetAPUTimer (uint16 Address, uint8 byte)
{
    (IAPU.RAM) [Address] = byte;
	
    switch (Address)
    {
    case 0xfa:
		if (((APUPack.APU.TimerTarget) [0] = (IAPU.RAM) [0xfa]) == 0)
			(APUPack.APU.TimerTarget) [0] = 0x100;
		(APUPack.APU.TimerValueWritten) [0] = TRUE;
		break;
    case 0xfb:
		if (((APUPack.APU.TimerTarget) [1] = (IAPU.RAM) [0xfb]) == 0)
			(APUPack.APU.TimerTarget) [1] = 0x100;
		(APUPack.APU.TimerValueWritten) [1] = TRUE;
		break;
    case 0xfc:
		if (((APUPack.APU.TimerTarget) [2] = (IAPU.RAM) [0xfc]) == 0)
			(APUPack.APU.TimerTarget) [2] = 0x100;
		(APUPack.APU.TimerValueWritten) [2] = TRUE;
		break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
uint8 S9xGetAPUDSP ()
{
//WAIT4MIXING()
    uint8 reg = (IAPU.RAM) [0xf2] & 0x7f;
    uint8 byte = APUPack.APU.DSP[reg];
	
    switch (reg)
    {
    case APU_KON:
		break;
    case APU_KOFF:
		break;
    case APU_OUTX + 0x00:
    case APU_OUTX + 0x10:
    case APU_OUTX + 0x20:
    case APU_OUTX + 0x30:
    case APU_OUTX + 0x40:
    case APU_OUTX + 0x50:
    case APU_OUTX + 0x60:
    case APU_OUTX + 0x70:
		/*if(Settings.FakeMuteFix)
		{
			// hack that is off by default: fixes Terranigma desync
			return (0);
		}
		else
		{*/
			if (SoundData.channels [reg >> 4].state == SOUND_SILENT)
				return (0);
			return (int8) (SoundData.channels [reg >> 4].out_sample >> 8);
		//}
    case APU_ENVX + 0x00:
    case APU_ENVX + 0x10:
    case APU_ENVX + 0x20:
    case APU_ENVX + 0x30:
    case APU_ENVX + 0x40:
    case APU_ENVX + 0x50:
    case APU_ENVX + 0x60:
    case APU_ENVX + 0x70:
		return ((uint8) S9xGetEnvelopeHeight (reg >> 4));

    case APU_ENDX:
		// To fix speech in Magical Drop 2 6/11/00
		//	APUPack.APU.DSP[APU_ENDX] = 0;
		break;
    default:
		break;
    }
    return (byte);
}

bool8 S9xInitAPU ()
{
#ifdef PSP
	uint8 *apu_ram;
#ifdef ME_SOUND
	uint8 *apu_vars;
#else
	uint8 *apu_ramc;
#endif


	  apu_ram = (uint8 *) malloc (0x10000 );
	  //malloc(0x10);
//    apu_vars=(uint8*)malloc(4*10+65536*4*2+65536*2*2);

//		IAPUuncached = (struct SIAPU *)UNCACHE_PTR(malloc(sizeof(struct SIAPU )));
//    APUuncached = (struct SAPU *)UNCACHE_PTR(malloc(sizeof(struct SAPU )));
//    APURegistersUncached = (struct SAPURegisters *)UNCACHE_PTR(malloc(sizeof(struct SAPURegisters )));
    //azz
    //IAPU = (struct SIAPU *)(malloc(sizeof(struct SIAPU )));
    //APU = (struct SAPU *)(malloc(sizeof(struct SAPU )));
    //APURegisters = (struct SAPURegisters *)(malloc(sizeof(struct SAPURegisters )));
#ifdef ME_SOUND
    apu_vars=(uint8*)malloc(4*10+65536*4*2+65536*2);
    apu_cycles_left_p=(int*)UNCACHE_PTR(((int)apu_vars)+4*0);
//    apu_glob_cycles_p=(int*)UNCACHE_PTR(((int)apu_vars)+4*1);
//    apu_init_after_load_=(int*)UNCACHE_PTR(((int)apu_vars)+4*2);
//    apu_event1_cpt1_p=(int*)UNCACHE_PTR(((int)apu_vars)+4*3);
#undef apu_glob_cycles
#undef apu_event1_cpt1
    apu_glob_cycles_p=&stAPUEvents_p->apu_glob_cycles;
    apu_event1_cpt1_p=&stAPUEvents_p->apu_event1_cpt1;
    apu_event2_cpt1_p=(int*)UNCACHE_PTR(((int)apu_vars)+4*4);
    apu_event1_cpt2_p=(int*)UNCACHE_PTR(((int)apu_vars)+4*5);
    apu_event2_cpt2_p=(int*)UNCACHE_PTR(((int)apu_vars)+4*6);
    apu_can_execute_p=(int*)UNCACHE_PTR(((int)apu_vars)+4*7);
    apu_ram_write_cpt1_p=(int*)UNCACHE_PTR(((int)apu_vars)+4*8);
    apu_ram_write_cpt2_p=(int*)UNCACHE_PTR(((int)apu_vars)+4*9);
    apu_event1=(int*)UNCACHE_PTR(((int)apu_vars)+4*10);
    apu_event2=(int*)UNCACHE_PTR(((int)apu_vars)+4*10+APU_EVENT_SIZE*4);
    apu_ram_write_log=(unsigned short*)UNCACHE_PTR(((int)apu_vars)+4*10+APU_EVENT_SIZE*4*2);
#undef apu_cycles_left
#undef apu_glob_cycles
#undef apu_event1_cpt1
#undef apu_event2_cpt1
#undef apu_event1_cpt2
#undef apu_event2_cpt2
#undef apu_init_after_load
#undef apu_can_execute
#undef apu_ram_write_cpt1
#undef apu_ram_write_cpt2

 //apu_cycles_left_p=&stAPUEvents_p->apu_cycles_left;
 //apu_glob_cycles_p=&stAPUEvents_p->apu_glob_cycles;
 //apu_event1_cpt1_p=&stAPUEvents_p->apu_event1_cpt1;
 //apu_event2_cpt1_p=&stAPUEvents_p->apu_event2_cpt1;
 //apu_event1_cpt2_p=&stAPUEvents_p->apu_event1_cpt2;
 //apu_event2_cpt2_p=&stAPUEvents_p->apu_event2_cpt2;
 //apu_init_after_load_=&stAPUEvents_p->apu_init_after_load;
 //apu_can_execute_p=&stAPUEvents_p->apu_can_execute;
 //apu_ram_write_cpt1_p=&stAPUEvents_p->apu_ram_write_cpt1;
 //apu_ram_write_cpt2_p=&stAPUEvents_p->apu_ram_write_cpt2;
 //apu_event1=stAPUEvents_p->apu_event1;
 //apu_event2=stAPUEvents_p->apu_event2;
 //apu_ram_write_log=stAPUEvents_p->apu_ram_write_log;
#endif
    //force write		    
    sceKernelDcacheWritebackInvalidateAll();                
#ifdef ME_SOUND
    IAPU.RAM = (uint8*)(apu_ram);
    IAPUuncached.RAM = (uint8*)(apu_ram);
#else
    IAPUuncached.RAM = (uint8*)UNCACHE_PTR(apu_ram);        
 	apu_ramc = (uint8 *) malloc (0x10000 );
   IAPU.RAM = apu_ramc;
#endif

#else
    (IAPUuncached.RAM) = (uint8 *) malloc (0x10000);
    //(IAPUuncached.ShadowRAM) = (uint8 *) malloc (0x10000);
    //(IAPUuncached.CachedSamples) = (uint8 *) malloc (0x40000);
#endif // PSP
    
    if (!(IAPUuncached.RAM) /*|| !(IAPUuncached.ShadowRAM) || !(IAPUuncached.CachedSamples)*/){
			S9xDeinitAPU ();
			return (FALSE);
    }

		memset((IAPUuncached.RAM), 0, 0x10000);
	
    return (TRUE);
}
