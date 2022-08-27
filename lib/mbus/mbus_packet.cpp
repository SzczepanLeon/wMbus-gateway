/******************************************************************************
    Filename: mbus_packet.cpp
******************************************************************************/

#include <stdint.h>

#include "3outof6.hpp"
#include "crc.hpp"
#include "mbus_packet.hpp"
#include "mbus_defs.hpp"


//----------------------------------------------------------------------------------
//  Functions
//----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
//  uint16_t packetSize (uint8_t lField)
//
//  DESCRIPTION:
//    Returns the number of bytes in a Wireless MBUS packet from
//    the L-field. Note that the L-field excludes the L-field and the 
//    CRC fields
//
//  ARGUMENTS:  
//    uint8_t lField  - The L-field value in a Wireless MBUS packet
//
//  RETURNS
//    uint16_t        - The number of bytes in a wireless MBUS packet 
//----------------------------------------------------------------------------------
uint16_t packetSize (uint8_t lField)
{
  uint16_t nrBytes;
  uint8_t  nrBlocks;
  
  // The 2 first blocks contains 25 bytes when excluding CRC and the L-field
  // The other blocks contains 16 bytes when excluding the CRC-fields
  // Less than 26 (15 + 10) 
  if ( lField < 26 ) 
    nrBlocks = 2;
  else 
    nrBlocks = (((lField - 26) / 16) + 3);
  
  // Add all extra fields, excluding the CRC fields
  nrBytes = lField + 1;

  // Add the CRC fields, each block is contains 2 CRC bytes
  nrBytes += (2 * nrBlocks);
      
  return (nrBytes);
}




//----------------------------------------------------------------------------------
//  uint16_t byteSize (uint16_t packetSize)
//
//  DESCRIPTION:
//    Returns the total number of encoded bytes to receive or transmit, given the 
//    total number of bytes in a Wireless MBUS packet. 
//    In receive mode the postamble sequence and synchronization word is excluded
//    from the calculation.
//
//  ARGUMENTS:
//    uint16_t  packetSize  - Total number of bytes in the wireless MBUS packet
//
//  RETURNS
//    uint16_t  - The number of bytes of the encoded WMBUS packet
//----------------------------------------------------------------------------------
uint16_t byteSize (uint16_t packetSize)
{
  uint16_t tmodeVar;
  
  // T-mode
  // Data is 3 out of 6 coded 
  tmodeVar = (3*packetSize) / 2;
  
  // Receive mode
  // If packetsize is a odd number 1 extra byte   
  // that includes the 4-postamble sequence must be
  // read.    
  if (packetSize % 2)
    return (tmodeVar + 1);
  else 
    return (tmodeVar);
}



//----------------------------------------------------------------------------------
//  uint16_t decodeRXBytesTmode(uint8_t* pByte, uint8_t* pPacket, uint16_t packetSize)
//
//  DESCRIPTION:
//    Decode a TMODE packet into a Wireless MBUS packet. Checks for 3 out of 6
//    decoding errors and CRC errors.
//
//   ARGUMENTS:  
//    uint8_t *pByte        - Pointer to TMBUS packet
//    uint8_t *pPacket      - Pointer to Wireless MBUS packet
//    uint16_t packetSize   - Total Size of the Wireless MBUS packet
//
//   RETURNS:
//    PACKET_OK              0
//    PACKET_CODING_ERROR    1
//    PACKET_CRC_ERROR       2
//----------------------------------------------------------------------------------
uint16_t decodeRXBytesTmode(uint8_t* pByte, uint8_t* pPacket, uint16_t packetSize)
{
    
  uint16_t bytesRemaining;
  uint16_t bytesEncoded;
  uint16_t decodingStatus;
  uint16_t crc;               // Current CRC value
  uint16_t crcField;          // Current fields are a CRC field

    
  bytesRemaining = packetSize;
  bytesEncoded   = 0;
  crcField       = 0;
  crc            = 0;
      
  // Decode packet      
  while (bytesRemaining)
  {
    // If last byte
    if (bytesRemaining == 1)
    {
      decodingStatus = decode3outof6(pByte, pPacket, 1);
      
      // Check for valid 3 out of 6 decoding
      if ( decodingStatus != DECODING_3OUTOF6_OK)
        return (PACKET_CODING_ERROR);
      
      bytesRemaining  -= 1;
      bytesEncoded    += 1;
      
      // The last byte the low byte of the CRC field
     if (LO_UINT16(~crc) != *(pPacket ))
        return (PACKET_CRC_ERROR);
    }
         
    else
    {
      
      decodingStatus = decode3outof6(pByte, pPacket, 0);
      
      // Check for valid 3 out of 6 decoding
      if ( decodingStatus != DECODING_3OUTOF6_OK)
        return (PACKET_CODING_ERROR);
        
      bytesRemaining -= 2; 
      bytesEncoded  += 2;
      
      
      // Check if current field is CRC fields
      // - Field 10 + 18*n
      // - Less than 2 bytes
      if (bytesRemaining == 0)
        crcField = 1;
      else if ( bytesEncoded > 10 )
        crcField = !((bytesEncoded - 12) % 18);
      
      // Check CRC field
      if (crcField)
      {        
       if (LO_UINT16(~crc) != *(pPacket + 1 ))
        return (PACKET_CRC_ERROR);
       if (HI_UINT16(~crc) != *pPacket)
          return (PACKET_CRC_ERROR);
       
       crcField = 0;        
       crc = 0;
      }
      
      // If 1 bytes left, the field is the high byte of the CRC
      else if (bytesRemaining == 1)
      {
        crc = crcCalc(crc, *(pPacket));
        // The packet byte is a CRC-field
       if (HI_UINT16(~crc) != *(pPacket + 1))
        return (PACKET_CRC_ERROR);
      }

      // Perform CRC calculation           
      else
       {
        crc = crcCalc(crc, *(pPacket));
        crc = crcCalc(crc, *(pPacket + 1));
       }
   
      pByte += 3;
      pPacket += 2;
      
    }
  }
  
  return (PACKET_OK);
}


/***********************************************************************************
  Copyright 2008 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
***********************************************************************************/


