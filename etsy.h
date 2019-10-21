#ifndef ETSY_H_INCLUDED
#define ETSY_H_INCLUDED
#include "ApplicationMapping.h"
#include "etsy_service_mapping.h" // All applications must have their individual service mapping header files
#include <stdbool.h>

const int defined_payload_length = 13;


void EncodeForCreateListing(char* payloadArray,int loadArrayLength,Title title,Description description,
int quantity,double price, WhoMade whoMade,WhenMade whenMade,unsigned long int sT,bool isSupply)
{
 // ETSY POST LISTING for createListing function call


    Application application = Etsy;
    EtsyService service = CreateListing;


    if(loadArrayLength <= 0)
    {
        return;
    }

    else if (loadArrayLength!=defined_payload_length)
    {
        payloadArray[0] = '\0';
    }

    else // loadArrayLength == defined_payload_length
    {

        /*  Starting from index 0 of the array:

            index 0 (1 byte) = Application ID
            index 1 (1 byte) = Service ID
            index 2 (1 byte) = Code for 'Title'
            index 3 (1 byte) = Code for 'Description'
            index 4 (1 byte) = Quantity (values in range of [0,255])

            index 5-6 (2 bytes):

                # Price (flotat type; Storing integral & fractional parts separately)
                # Range of values:
                    Integral portion: [0,4095]
                    Fractional Portion: [0,15]

                # Bit Allocation Breakdown:

                    @ index 5 (1 byte):
                        All 8 bits are for (integral portion of the price) / 16

                    @ index 6 (1 byte):
                        From LMB side, bits(7-4): (integral portion of the price) % 16 // 2^4 - 1 = 15
                        From LMB side, bits(3-0):  fractional portion of the price [0,15]


            index 7 (1 byte):

                From LMB side, bits(7-4): Code for Who_Made (2^4 = 16 possible values)
                From LMB side, bits(3-1): Code for When_Made (2^3 = 8 possible values)
                From LMB side, bits(0): Code for is_supply(2^1 = 2 possible values (True/False))

            index 8-12 (5 bytes): Shipping template id (probably unique to each shop/ user???)
                Range of values: [0, 1.0995116 e^12]

        */

        // Initializing the array with null char at each index. Will set values accordingly.
        for(int i=0;i<defined_payload_length;i++)
        {
            *(payloadArray+i) = '\0'; // Equivalent binary code is 0b00000000
        }


        //------------------------- Encoding Operations Below ----------------------

        int payLoadIndex = 0;


        // Encoding Application id
        Application appId = 0;
        char app = 0b00000000;

        while (appId!=application)
        {
            app += 0b00000001;
            appId++;
        }

        // setting Application id in the payload array index = 0
        *(payloadArray+payLoadIndex) = app;
        payLoadIndex++; // Now index = 1

        //--------------------------------------------------------------------------

        // Encoding Service id
        EtsyService svcId = 0;
        char svc = 0b00000000;

        while(svcId!=service)
        {
            svc += 0b00000001;
            svcId++;
        }

        //Setting Service id in the payload array index = 1
        *(payloadArray+payLoadIndex) = svc;
        payLoadIndex++; // Now index = 2

        //--------------------------------------------------------------------------

        // Encoding Code for Title

        Title tempTitle = 0;
        char encoded_title = 0b00000000;

        while(tempTitle!=title)
        {
            encoded_title += 0b00000001;
            tempTitle++;
        }

        // Setting Code for Title field in the payLoad array index = 2
        *(payloadArray+payLoadIndex) = encoded_title;
        payLoadIndex++; // Now index = 3


        //--------------------------------------------------------------------------

        // Encoding Description

        Description tempDesc = 0;
        char encoded_description = 0b00000000;

        while(tempDesc!=description)
        {
            encoded_description += 0b00000001;
            tempDesc++;

        }

        // Setting Code for Description field in the payLoad array index = 3
        *(payloadArray+payLoadIndex) = encoded_description;
        payLoadIndex++; // Now index = 4


        //--------------------------------------------------------------------------

        // Encoding quantity

        if(quantity>255)
            quantity = 255; // Can represent only the max value in range of [0,2^8-1]

        int temp_qty = 0;
        char encoded_quantity = 0b00000000;

        while(temp_qty!=quantity)
        {
            encoded_quantity += 0b00000001;
            temp_qty++;
        }

        // Setting quantity field in the payLoad array index = 4
        *(payloadArray+payLoadIndex) = encoded_quantity;
        payLoadIndex++; // Now index = 5

        //--------------------------------------------------------------------------

        // Encoding price (float type) as a 16 bit number
        // (12 bits for integral portion, 4 bits for fractional portion)
        int integer_portion = (int) price;

        if(integer_portion>4095)
            integer_portion = 4095; // Can only represent values in range [0, (2^12)-1]

        price = price*100;

        int price_decimal_portion = ((int)price)%100;

        if(price_decimal_portion>15)
            price_decimal_portion = 15; // Can only represent values from 0 to (2^4)-1


        // Breaking down integral portion of the price to be stored
        // on the bits of index = 5 and from LMB side bits(7-4) of index = 6

        int price_integral_4LSdigits = integer_portion%16;// It goes into index = 6, from LMB side, into bits(7-4)

        int price_integral_8MSdigits = integer_portion/16; // Goes into index = 5 bits


        char priceMSdigits = 0b00000000;
        int tempMSdigits = 0;

        while(tempMSdigits!=price_integral_8MSdigits)
        {
            priceMSdigits += 0b00000001;
            tempMSdigits++;
        }


        // Setting up the integral portion of the price at index = 5
        *(payloadArray+payLoadIndex) = priceMSdigits;
        payLoadIndex++; // Now index = 6


        // Encoding & setting up remaining 4 least significant digits of the
        // integral portion of the price at bits[7-4] (from LMB side) at index = 6


        char priceLSdigits = 0b00000000;
        int tempLSdigits = 0;

        while(tempLSdigits!=price_integral_4LSdigits)
        {
            priceLSdigits += 0b00000001;
            tempLSdigits++;
        }

        priceLSdigits = priceLSdigits<<4; // Shifting 4 spcaes to the left

        // Setting up the bits in the payLoad array index = 6 (bit 7 to bit 4)
        // using bitwise OR between *(payloadArray+payLoadIndex) and priceLSdigits
        *(payloadArray+payLoadIndex) = *(payloadArray+payLoadIndex) | priceLSdigits;



        // Encoding & setting up the fractional portion of the price in the payLoad array
        // index = 6 (bit 3 to bit 0)


        char price_decimal_encoded = 0b00000000;
        int tempDecimal = 0;

        while(tempDecimal!=price_decimal_portion)
        {
            price_decimal_encoded += 0b00000001;
            tempDecimal++;
        }

        // Setting up the value in the payLoad array using bitwise OR
        // operation as *(payLoadArray+payLoadIndex) = *(payLoadArray+payLoadIndex) | price_decimal_encoded
        *(payloadArray+payLoadIndex) = *(payloadArray+payLoadIndex) | price_decimal_encoded;
        payLoadIndex++; // Now index = 7


        //-----------------------------------------------------------------------------------------------------


        // Encoding Who_Made, When_Made, and isSupply information at index = 7 as below:
        // bit 7 - bit 4: Who_Made, bit 3 - bit 1: When_Made, bit 0: is_Supply (1-Yes/0-No)


        char who_made = 0b00000000;
        WhoMade tempWM = 0;
        while(tempWM!=whoMade)
        {
            who_made += 0b00000001;
            tempWM++;
        }

        // Shifting 4 bits to the left
        who_made = who_made<<4;

        // Setting the value in the payLoad array
        *(payloadArray+payLoadIndex) = *(payloadArray+payLoadIndex) | who_made;


        char when_made = 0b00000000;
        WhenMade tempWhenMade = 0;

        while(tempWhenMade!=whenMade)
        {
            when_made += 0b00000001;
            tempWhenMade++;
        }

        // SHifting 1 bit to the left
        when_made = when_made<<1;

        // Setting the value in the payLoad array
        *(payloadArray+payLoadIndex) = *(payloadArray+payLoadIndex) | when_made;


        char is_supply = 0b00000000;

        if(isSupply==true)
            is_supply = 0b00000001;

        // Setting the value in the payLoad array
        *(payloadArray+payLoadIndex) = *(payloadArray+payLoadIndex) | is_supply;
        payLoadIndex++; // Now index = 8


        //---------------------------------------------------------------------------------------------------


        // Encoding shipping template id as a 40 bit number.

        // Breaking it in 5 parts to be stored separately at indices 8,9,10,11, and 12.
        // part5 = Most significant 8 bits of shipping template -> Will go at index 8
        // part4 = Next siginificant 8 bits of shipping template -> Will go at index 9
        // part3 = Next siginificant 8 bits of shipping template -> Will go at index 10
        // part2 = Next siginificant 8 bits of shipping template -> Will go at index 11
        // part1 = Least siginificant 8 bits of shipping template -> Will go at index 12

        int part1 = sT%256;

        sT = sT/256;

        int part2 = sT%256;

        sT = sT/256;

        int part3 = sT%256;

        sT = sT/256;

        int part4 = sT%256;

        sT = sT/256;

        int part5 = sT;


        char part5_encoded = 0b00000000;
        int temp = 0;
        while(temp!=part5)
        {
            part5_encoded += 0b00000001;
            temp++;
        }

        // Setting part5 in payLoad array at index = 8
        *(payloadArray+payLoadIndex) = part5_encoded;
        payLoadIndex++; // Now index = 9


        char part4_encoded = 0b00000000;
        temp = 0;
        while(temp!=part4)
        {
            part4_encoded += 0b00000001;
            temp++;
        }

        // Setting part4 in payLoad array at index = 9
        *(payloadArray+payLoadIndex) = part4_encoded;
        payLoadIndex++; // Now index = 10



        char part3_encoded = 0b00000000;
        temp = 0;
        while(temp!=part3)
        {
            part3_encoded += 0b00000001;
            temp++;
        }

        // Setting part3 in payLoad array at index = 10
        *(payloadArray+payLoadIndex) = part3_encoded;
        payLoadIndex++; // Now index = 11



        char part2_encoded = 0b00000000;
        temp = 0;
        while(temp!=part2)
        {
            part2_encoded += 0b00000001;
            temp++;
        }

        // Setting part2 in payLoad array at index = 11
        *(payloadArray+payLoadIndex) = part2_encoded;
        payLoadIndex++; // Now index = 12


        char part1_encoded = 0b00000000;
        temp = 0;
        while(temp!=part1)
        {
            part1_encoded += 0b00000001;
            temp++;
        }

        // Setting part4 in payLoad array at index = 12
        *(payloadArray+payLoadIndex) = part1_encoded;

    } // end of condition loadArrayLength == defined_payload_length


} // function CreateListing(...) ends here


#endif // ETSY_H_INCLUDED
