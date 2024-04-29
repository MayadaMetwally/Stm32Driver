/*This header file contains several sshifting and masking configurations to be used in reading and writing registers*/

#ifndef MASKS_H
#define MASKS_H

/*Bits Offset factor in Shift*/
#define Reg_Offset_2bit      2
#define Reg_Offset_4bit      4
#define Reg_Offset_8bit      8

/*Register 1-Bit Shift*/
#define Reg_bit0            0
#define Reg_bit1            1
#define Reg_bit2            2
#define Reg_bit3            3
#define Reg_bit4            4
#define Reg_bit5            5
#define Reg_bit6            6
#define Reg_bit7            7
#define Reg_bit8            8
#define Reg_bit9            9
#define Reg_bit10           10
#define Reg_bit11           11
#define Reg_bit12           12
#define Reg_bit13           13
#define Reg_bit14           14
#define Reg_bit15           15
#define Reg_bit16           16
#define Reg_bit17           17
#define Reg_bit18           18
#define Reg_bit19           19
#define Reg_bit20           20
#define Reg_bit21           21
#define Reg_bit22           22
#define Reg_bit23           23
#define Reg_bit24           24
#define Reg_bit25           25
#define Reg_bit26           26
#define Reg_bit27           27
#define Reg_bit28           28
#define Reg_bit29           29
#define Reg_bit30           30
#define Reg_bit31           31

/*Register 2-Bit Shift*/
#define Reg_bit0_1           0
#define Reg_bit2_3           2
#define Reg_bit4_5           4
#define Reg_bit6_7           6
#define Reg_bit8_9           8
#define Reg_bit10_11         10
#define Reg_bit12_13         12
#define Reg_bit14_15         14
#define Reg_bit16_17         16
#define Reg_bit18_19         18
#define Reg_bit20_21         20
#define Reg_bit22_23         22
#define Reg_bit24_25         24
#define Reg_bit26_27         26
#define Reg_bit28_29         28
#define Reg_bit30_31         30        

/*1-bit Masks for a 32bit register*/
#define Reg_Mask_1bit       0x00000001
#define Reg_Mask_bit0       0x00000001
#define Reg_Mask_bit1       0x00000002
#define Reg_Mask_bit2       0x00000004
#define Reg_Mask_bit3       0x00000008
#define Reg_Mask_bit4       0x00000010
#define Reg_Mask_bit5       0x00000020
#define Reg_Mask_bit6       0x00000040
#define Reg_Mask_bit7       0x00000080
#define Reg_Mask_bit8       0x00000100
#define Reg_Mask_bit9       0x00000200
#define Reg_Mask_bit10      0x00000400
#define Reg_Mask_bit11      0x00000800
#define Reg_Mask_bit12      0x00001000
#define Reg_Mask_bit13      0x00002000
#define Reg_Mask_bit14      0x00004000
#define Reg_Mask_bit15      0x00008000
#define Reg_Mask_bit16      0x00010000
#define Reg_Mask_bit17      0x00020000
#define Reg_Mask_bit18      0x00040000
#define Reg_Mask_bit19      0x00080000
#define Reg_Mask_bit20      0x00100000
#define Reg_Mask_bit21      0x00200000
#define Reg_Mask_bit22      0x00400000
#define Reg_Mask_bit23      0x00800000
#define Reg_Mask_bit24      0x01000000
#define Reg_Mask_bit25      0x02000000
#define Reg_Mask_bit26      0x04000000
#define Reg_Mask_bit27      0x08000000
#define Reg_Mask_bit28      0x10000000
#define Reg_Mask_bit29      0x20000000
#define Reg_Mask_bit30      0x40000000
#define Reg_Mask_bit31      0x80000000


/*2-bit Masks for a 32bit register*/
#define Reg_Mask_2bits      0x00000003
#define Reg_Mask2_bit01     0x00000003
#define Reg_Mask2_bit23     0x0000000C
#define Reg_Mask2_bit45     0x00000030
#define Reg_Mask2_bit67     0x000000C0
#define Reg_Mask2_bit89     0x00000300
#define Reg_Mask2_bit1011   0x00000C00
#define Reg_Mask2_bit1213   0x00003000
#define Reg_Mask2_bit1415   0x0000C000
#define Reg_Mask2_bit1617   0x00030000
#define Reg_Mask2_bit1819   0x000C0000
#define Reg_Mask2_bit2021   0x00300000
#define Reg_Mask2_bit2223   0x00C00000
#define Reg_Mask2_bit2425   0x03000000
#define Reg_Mask2_bit2627   0x0C000000
#define Reg_Mask2_bit2829   0x30000000
#define Reg_Mask2_bit3031   0xC0000000

/*4-bit Masks for a 32bit register*/
#define Reg_Mask_4bits      0x0000000F
#define Reg_Mask4_bit03     0x0000000F
#define Reg_Mask4_bit47     0x000000F0
#define Reg_Mask4_bit811    0x00000F00
#define Reg_Mask4_bit1215   0x0000F000
#define Reg_Mask4_bit41620  0x000F0000
#define Reg_Mask4_bit52124  0x00F00000
#define Reg_Mask4_bit62528  0x0F000000
#define Reg_Mask4_bit72931  0xF0000000

#define CONCAT(num1, num2) ((uint64_t)(num1) << 32 | (uint32_t)(num2))

#define EXTRACT_1BIT(num, pos)   (((num) >> (pos)) & 0x01)
#define EXTRACT_2BIT(num, pos)   (((num) >> (pos)) & 0x03)

#endif