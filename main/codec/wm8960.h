//
// Created by ilia on 13.1.23..
//

#ifndef MAIN_WM8960_H
#define MAIN_WM8960_H

#include "driver/i2c.h"
#include "driver/i2s_std.h"
#include "driver/i2s_common.h"
#include "driver/i2s_types.h"
#include <stdint.h>

#define WM8960_ADDRESS 0x1A //AUDIO CODEC ADDRESS

#define SAMPLE_RATE     (44100)
#define WAVE_FREQ_HZ    (100)
#define PI              (3.14159265)
#define SAMPLE_PER_CYCLE (SAMPLE_RATE/WAVE_FREQ_HZ)
#define EXAMPLE_BUFF_SIZE (SAMPLE_PER_CYCLE*2)

#define NUM_CHANNELS 2
#define BIT_PER_SAMPLE 16
#define BYTE_RATE (SAMPLE_RATE*NUM_CHANNELS*BIT_PER_SAMPLE/8)

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

void i2s_init(void);
void i2c_init(void);
esp_err_t wm8960_writeReg(uint8_t reg, uint16_t dat);
void wm8960Init();
void i2s_example_write_task(void *args);

///////////////////////////////////
enum {
    R0_LEFT_INPUT_VOLUME_ADR,
    R1_RIGHT_INPUT_VOLUME_ADR,
    R2_LOUT1_VOLUME_ADR,
    R3_ROUT1_VOLUME_ADR,
    R4_CLOCKING_1_ADR,
    R5_ADC_DAC_CONTROL_CTR1_ADR,
    R6_ADC_DAC_CONTROL_CTR2_ADR,
    R7_AUDIO_INTERFACE_1_ADR,
    R8_CLOCKING_2_ADR,
    R9_AUDIO_INTERFACE_ADR,
    R10_LEFT_DAC_VOLUME_ADR,
    R11_RIGHT_DAC_VOLUME_ADR,
    R12_RESERVED,
    R13_RESERVED,
    R14_RESERVED,
    R15_RESET_ADR,
    R16_3D_CONTROL_ADR,
    R17_ALC1_ADR,
    R18_ALC2_ADR,
    R19_ALC3_ADR,
    R20_NOISE_GATE_ADR,
    R21_LEFT_ADC_VOLUME_ADR,
    R22_RIGHT_ADC_VOLUME_ADR,
    R23_ADDITIONAL_CONTROL_1_ADR,
    R24_ADDITIONAL_CONTROL_2_ADR,
    R25_PWR_MGMT_1_ADR,
    R26_PWR_MGMT_2_ADR,
    R27_ADDITIONAL_CONTROL_3_ADR,
    R28_ANTI_POP_1_ADR,
    R29_ANTI_POP_2_ADR,
    R30_RESERVED,
    R31_RESERVED,
    R32_ADCL_SIGNAL_PATH,
    R33_ADCR_SIGNAL_PATH,
    R34_LEFT_OUT_MIX_2,
    R35_RESERVED,
    R36_RESERVED,
    R37_RIGHT_OUT_MIX_2,
    R38_MONO_OUT_MIX_1,
    R39_MONO_OUT_MIX_2,
    R40_LOUT2_VOLUME,
    R41_ROUT2_VOLUME,
    R42_MONOOUT_VOLUME,
    R43_INPUT_BOOST_MIXER_1,
    R44_INPUT_BOOST_MIXER_2,
    R45_BYPASS_1,
    R46_BYPASS_2,
    R47_PWR_MGMT_3,
    R48_ADDITONAL_CTRL_4,
    R49_CLASS_D_CTRL_1,
    R50_RESERVED,
    R51_CLASS_D_CTRL_3,
    R52_PLL_1_ADDR,
    R53_PLL_2_ADDR,
    R54_PLL_3_ADDR,
    R55_PLL_4_ADDR
};


typedef struct R0_LEFT_INPUT_VOLUME_t {
    uint16_t LINVOL: 6; //Bits 5:0
    uint16_t LIZC: 1; //Bits 6
    uint16_t LINMUTE: 1; //Bits 7
    uint16_t IPUV: 1; //Bits 8

} __attribute__((packed, aligned(2))) R0_LEFT_INPUT_VOLUME_t;


typedef struct R1_RIGHT_INPUT_VOLUME_t {
    uint16_t RINVOL: 6; //Bits 5:0
    uint16_t RIZC: 1; //Bits 6
    uint16_t RINMUTE: 1; //Bits 7
    uint16_t IPUV: 1; //Bits 8

} __attribute__((packed, aligned(2))) R1_RIGHT_INPUT_VOLUME_t;


typedef struct R2_LOUT1_VOLUME_t {
    uint16_t LOUT1VOL: 7; //Bits 6:0
    uint16_t LO1ZC: 1; //Bits 7
    uint16_t OUT1VU: 1; //Bits 8
} __attribute__((packed, aligned(2))) R2_LOUT1_VOLUME_t;

typedef struct R3_ROUT1_VOLUME_t {
    uint16_t ROUT1VOL: 7; //Bits 6:0
    uint16_t RO1ZC: 1; //Bits 7
    uint16_t OUT1VU: 1; //Bits 8
} __attribute__((packed, aligned(2))) R3_ROUT1_VOLUME_t;

typedef struct R4_CLOCKING_1_t {

    uint16_t ADCDIV: 3; //Bits 8:6
    uint16_t DACDIV: 3; //Bits 5:3
    uint16_t SYSCLKDIV: 2; //Bits 2:1
    uint16_t CLKSEL: 1; //Bits 0
} __attribute__((packed, aligned(2))) R4_CLOCKING_1_t;

typedef struct R5_ADC_DAC_CONTROL_CTR1_t {

    uint16_t ADCHPD: 1; //Bits 0
    uint16_t DEEMPH: 2; //Bits 2:1
    uint16_t DACMU: 1; //Bits 3
    uint16_t R5RES_4: 1;
    uint16_t ADCPOL: 2; //Bits 6:5
    uint16_t DACDIV2: 1; //Bits 7
    uint16_t R5RES_8: 1;
}__attribute__((packed, aligned(2))) R5_ADC_DAC_CONTROL_CTR1_t;

typedef struct R6_ADC_DAC_CONTROL_CTR2_t {

    uint16_t DACSLOPE: 1; //Bits 1
    uint16_t DACMR: 1; //Bits 2
    uint16_t DACSMM: 1; //Bits 3
    uint16_t RESERV4: 1; //      RESERVED
    uint16_t DACPOL: 2; //Bits 6:5
    uint16_t RESERV7: 1; //      RESERVED
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R6_ADC_DAC_CONTROL_CTR2_t;

typedef struct R7_AUDIO_INTERFACE_t {
    uint16_t FORMAT: 2; // bit 1:0
    uint16_t WL: 2; //
    uint16_t LRP: 1; // bit 4
    uint16_t DLRSWAP: 1; // bit 5
    uint16_t MS: 1; // bit 6
    uint16_t BCLKINV: 1; // bit 7
    uint16_t ALRSWAP: 1; // bit 8
}__attribute__((packed, aligned(2))) R7_AUDIO_INTERFACE_t;

typedef struct R8_CLOCKING_2_t {
    uint16_t BCLKDIV: 4; // bit 3:0
    uint16_t RESERV54: 2; //      RESERVED
    uint16_t DCLKDIV: 3; // bit 8.. 6
}__attribute__((packed, aligned(2))) R8_CLOCKING_2_t;

typedef struct R9_AUDIO_INTERFACE_t {
    uint16_t LOOPBACK: 1; // bit 0
    uint16_t ADCCOMP: 2; // bits 1:0
    uint16_t DACCOMP: 2; // bits 1:0
    uint16_t WL8: 1; // bit 5
    uint16_t ALRCGPIO: 1; // bit 6
    uint16_t RESERV87: 2; //      RESERVED
}__attribute__((packed, aligned(2))) R9_AUDIO_INTERFACE_t;

typedef struct R10_LEFT_DAC_VOLUME_t {
    uint16_t LDACVOL: 8; // bit 7:0
    uint16_t DACVU: 1; // bit 8
}__attribute__((packed, aligned(2))) R10_LEFT_DAC_VOLUME_t;

typedef struct R11_RIGHT_DAC_VOLUME_t {
    uint16_t RDACVOL: 8; // bit 7:0
    uint16_t DACVU: 1; // bit 8
}__attribute__((packed, aligned(2))) R11_RIGHT_DAC_VOLUME_t;

// typedef struct R15_RESET_t{};

typedef struct R16_3D_CONTROL_t {
    uint16_t D3EN: 1; // bit 0
    uint16_t D3DEPTH: 4; // bits 3:0
    uint16_t D3LC: 1; // bit 5
    uint16_t D3UC: 1; // bit 6
    uint16_t RESERV7: 1; //      RESERVED
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R16_3D_CONTROL_t;

typedef struct R17_ALC1_t {
    uint16_t ALCL: 4; // bits 3:0
    uint16_t MAXGAIN: 3; // bits 2:0
    uint16_t ALCSEL: 2; // bits 1:0
}__attribute__((packed, aligned(2))) R17_ALC1_t;

typedef struct R18_ALC2_t {
    uint16_t HLD: 4; // bits 3:0
    uint16_t MINGAIN: 3; // bits 6..4
    uint16_t RESERV7: 1; //      RESERVED
    uint16_t RESERV8: 1; //      RESERVED default=1 why? :-/
}__attribute__((packed, aligned(2))) R18_ALC2_t;

typedef struct R19_ALC3_t {
    uint16_t ATK: 4; // bits 3:0
    uint16_t DCY: 4; // bits 3:0
    uint16_t ALCMODE: 1; //bit 8
}__attribute__((packed, aligned(2))) R19_ALC3_t;

typedef struct R20_NOISE_GATE_t {
    uint16_t NGAT: 1; // bit 0
    uint16_t RESERV21: 2; //      RESERVED
    uint16_t NGTH: 5; // bit 7..3
    uint16_t RESERV8: 1; //      RESERVED

}__attribute__((packed, aligned(2))) R20_NOISE_GATE_t;

typedef struct R21_LEFT_ADC_VOLUME_t {
    uint16_t LADCVOL: 8; // bits 7:0
    uint16_t ADCVU: 1; // bit  8
}__attribute__((packed, aligned(2))) R21_LEFT_ADC_VOLUME_t;

typedef struct R22_RIGHT_ADC_VOLUME_t {
    uint16_t RADCVOL: 8; // bits 7:0
    uint16_t ADCVU: 1; // bit  8
}__attribute__((packed, aligned(2))) R22_RIGHT_ADC_VOLUME_t;

typedef struct R23_ADDITIONAL_CONTROL_1_t {
    uint16_t TOEN: 1; // bit 1
    uint16_t TOCLKSEL: 1; // bit  2
    uint16_t DATSEL: 2; // bits 1:0
    uint16_t DMONOMIX: 1; // bit 4
    uint16_t VSEL: 2; // bits 1:0
    uint16_t TSDEN: 1; // bit 8
}__attribute__((packed, aligned(2))) R23_ADDITIONAL_CONTROL_1_t;

typedef struct R24_ADDITIONAL_CONTROL_2_t {
    uint16_t LRCM: 1; // bit 2
    uint16_t TRIS: 1; // bit 3
    uint16_t HPSWPOL: 1; // bit 5
    uint16_t HPSWEN: 1; // bit 6
    uint16_t RESERV7: 1; //      RESERVED
    uint16_t RESERV8: 1; //      RESERVED

}__attribute__((packed, aligned(2))) R24_ADDITIONAL_CONTROL_2_t;

typedef struct R25_PWR_MGMT_1_t {
    uint16_t DIGENB: 1; // bit 0
    uint16_t MICB: 1; // bit 1
    uint16_t ADCR: 1; // bit 2
    uint16_t ADCL: 1; // bit 3
    uint16_t AINR: 1; // bit 4
    uint16_t AINL: 1; // bit 5
    uint16_t VREF: 1; // bit 6
    uint16_t VMIDSEL: 2; // bit 1:0
}__attribute__((packed, aligned(2))) R25_PWR_MGMT_1_t;

typedef struct R26_PWR_MGMT_2_t {
    uint16_t PLL_EN: 1; // bit 0
    uint16_t OUT3: 1; // bit 1
    uint16_t RESERV: 1; //      RESERVED
    uint16_t SPKR: 1; // bit 3
    uint16_t SPKL: 1; // bit 4
    uint16_t ROUT1: 1; // bit 5
    uint16_t LOUT1: 1; // bit 6
    uint16_t DACR: 1; // bit 7
    uint16_t DACL: 1; // bit 8
}__attribute__((packed, aligned(2))) R26_PWR_MGMT_2_t;

typedef struct R27_ADDITIONAL_CONTROL_3_t {
    uint16_t ADC_ALC_SR: 3; // bits 2:0
    uint16_t OUT3CAP: 1; // bit 3
    uint16_t RESERV4: 1; //      RESERVED
    uint16_t RESERV5: 1; //      RESERVED
    uint16_t VROI: 1; // bit 6
    uint16_t RESERV7: 1; //      RESERVED
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R27_ADDITIONAL_CONTROL_3_t;

typedef struct R28_ANTI_POP_1_t {
    uint16_t HPSTBY: 1; // bit 0
    uint16_t RESERVED: 1;
    uint16_t SOFT_ST: 1; // bit 2
    uint16_t BUFIOEN: 1; // bit 3
    uint16_t BUFDCOPEN: 1; // bit 4
    uint16_t RESERV65: 2; //      RESERVED
    uint16_t POBCTRL: 1; // bit 7
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R28_ANTI_POP_1_t;

typedef struct R29_ANTI_POP_2_t {
    uint16_t RESERV30: 4; //      RESERVED
    uint16_t DRES: 2; // bits 5..4
    uint16_t DISOP: 1; // bit 5
    uint16_t RESERV87: 2; //      RESERVED
}__attribute__((packed, aligned(2))) R29_ANTI_POP_2_t;

typedef struct R32_ADCL_SIGNAL_PATH_t {
    uint16_t RESERV20: 3; //      RESERVED
    uint16_t LMIC2B: 1; // bit 3
    uint16_t LMICBOOST: 2; //  4 5
    uint16_t LMP2: 1; // bit 6
    uint16_t LMP3: 1; // bit 7
    uint16_t LMN1: 1; // bit 8
}__attribute__((packed, aligned(2))) R32_ADCL_SIGNAL_PATH_t;

typedef struct R33_ADCR_SIGNAL_PATH_t {
    uint16_t RESERV20: 3; //      RESERVED
    uint16_t RMIC2B: 1; // bit 3
    uint16_t RMICBOOST: 2; //
    uint16_t RMP2: 1; // bit 6
    uint16_t RMP3: 1; // bit 7
    uint16_t RMN1: 1; // bit 8
}__attribute__((packed, aligned(2))) R33_ADCR_SIGNAL_PATH_t;

typedef struct R34_LEFT_OUT_MIX_1_t {
    uint16_t RESERV30: 4; //      RESERVED
    uint16_t LI2LOVOL: 3; // bits 6  4
    uint16_t LI2LO: 1; // bit 7
    uint16_t LD2LO: 1; // bit 8
}__attribute__((packed, aligned(2))) R34_LEFT_OUT_MIX_1_t;

typedef struct R37_RIGHT_OUT_MIX_2_t {
    uint16_t RESERV30: 4; //      RESERVED
    uint16_t RI2ROVOL: 3; // bits 6  4
    uint16_t RI2RO: 1; // bit 7
    uint16_t RD2RO: 1; // bit 8
}__attribute__((packed, aligned(2))) R37_RIGHT_OUT_MIX_2_t;

typedef struct R38_MONO_OUT_MIX_1_t {
    uint16_t RESERV60: 7; //      RESERVED
    uint16_t L2MO: 1; // bit 7
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R38_MONO_OUT_MIX_1_t;

typedef struct R39_MONO_OUT_MIX_2_t {
    uint16_t RESERV60: 7; //      RESERVED
    uint16_t R2MO: 1; // bit 7
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R39_MONO_OUT_MIX_2_t;

typedef struct R40_LOUT2_VOLUME_t {
    uint16_t SPKLVOL: 7; // bits 6:0
    uint16_t SPKLZC: 1; // bit 7
    uint16_t SPKVU: 1; // bit 8
}__attribute__((packed, aligned(2))) R40_LOUT2_VOLUME_t;

typedef struct R41_ROUT2_VOLUME_t {
    uint16_t SPKRVOL: 7; // bits 6:0
    uint16_t SPKLZC: 1; // bit 7
    uint16_t SPKVU: 1; // bit 8
}__attribute__((packed, aligned(2))) R41_ROUT2_VOLUME_t;

typedef struct R42_MONOOUT_VOLUME_t {
    uint16_t RESERV50: 6; //      RESERVED
    uint16_t MOUTVOL: 1; //bit 6
    uint16_t RESERV87: 2; //      RESERVED
}__attribute__((packed, aligned(2))) R42_MONOOUT_VOLUME_t;

typedef struct R43_INPUT_BOOST_MIXER_1_t {
    uint16_t RESERV0: 1; //      RESERVED
    uint16_t LIN2BOOST: 3; // bits 2:0
    uint16_t LIN3BOOST: 3; // bits 2:0
    uint16_t RESERV87: 2; //      RESERVED
}__attribute__((packed, aligned(2))) R43_INPUT_BOOST_MIXER_1_t;

typedef struct R44_INPUT_BOOST_MIXER_2_t {
    uint16_t RESERV0: 1; //      RESERVED
    uint16_t RIN2BOOST: 3; // bits 2:0
    uint16_t RIN3BOOST: 3; // bits 2:0
    uint16_t RESERV87: 2; //      RESERVED
}__attribute__((packed, aligned(2))) R44_INPUT_BOOST_MIXER_2_t;

typedef struct R45_BYPASS_1_t {
    uint16_t RESERV30: 4; //      RESERVED
    uint16_t LB2LOVOL: 3; // bits 6  4
    uint16_t LB2LO: 1; // bit 7
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R45_BYPASS_1_t;

typedef struct R46_BYPASS_2_t {
    uint16_t RESERV30: 4; //      RESERVED
    uint16_t RB2ROVOL: 3; //
    uint16_t RB2RO: 1; //
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R46_BYPASS_2_t;

typedef struct R47_PWR_MGMT_3_t {
    uint16_t RESERV0: 1; //      RESERVED
    uint16_t RESERV1: 1; //      RESERVED
    uint16_t ROMIX: 1; // bit 2
    uint16_t LOMIX: 1; // bit 3
    uint16_t RMIC: 1; // bit 4
    uint16_t LMIC: 1; // bit 5
    uint16_t RESERV6: 1; //      RESERVED
    uint16_t RESERV7: 1; //      RESERVED
    uint16_t RESERV8: 1; //      RESERVED

}__attribute__((packed, aligned(2))) R47_PWR_MGMT_3_t;

typedef struct R48_ADDITIONAL_CONTROL_4_t {
    uint16_t MBSEL: 1; // bit 0
    uint16_t TSENSEN: 1; // bit 1
    uint16_t HPSEL: 2; // bits 1:0
    uint16_t GPIOSEL: 3; // bits 2:0
    uint16_t GPIOPOL: 1; // bit 7
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R48_ADDITIONAL_CONTROL_4_t;

typedef struct R49_CLASS_D_CONTROL_1_t {
    uint16_t RESERV50: 6; //      RESERVED default = 110111
    uint16_t SPK_OP_EN: 2; // bits 7  6
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R49_CLASS_D_CONTROL_1_t;

typedef struct R51_CLASS_D_CONTROL_3_t {
    uint16_t ACGAIN: 3; // bits 2:0
    uint16_t DCGAIN: 3; // bits 2:0
    uint16_t RESERV86: 3; //      RESERVED default = 010
}__attribute__((packed, aligned(2))) R51_CLASS_D_CONTROL_3_t;

typedef struct R52_PLL_N_t {
    uint16_t PLLN: 4; // bits 3:0
    uint16_t PLLRESCALE: 1; // bit 4
    uint16_t SDM: 1; // bit 5
    uint16_t OPCLKDIV: 3; // bits 2:0
}__attribute__((packed, aligned(2))) R52_PLL_N_t;

typedef struct R53_PLL_K_1_t {
    uint16_t PLLK: 8; // bits 23:16
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R53_PLL_K_1_t;

typedef struct R54_PLL_K_2_t {
    uint16_t PLLK: 8; // bits 15:8 0..7
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R54_PLL_K_2_t;

typedef struct R55_PLL_K_3_t {
    uint16_t PLLK: 8; // bits 7:0
    uint16_t RESERV8: 1; //      RESERVED
}__attribute__((packed, aligned(2))) R55_PLL_K_3_t;


#endif //MAIN_WM8960_H
