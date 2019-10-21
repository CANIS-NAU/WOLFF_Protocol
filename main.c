#include <stdio.h>
#include "ApplicationMapping.h"
#include "ServiceMapping.h"
#include "EtsyDecodeClientMessage.h"

int main()
{
    // This empty, uninitialized char array will be passed to the
    // encoding function to be used as the payload array where
    // encoded bits will be stored.
    char* encodedMessage = (char*) malloc(13*(sizeof(char)));


    // Intializing variables with sample parameter values for etsy create_listing

    Title t = bracelet; // Defined in etsy_service_mapping.h

    Description d = handmade; // Defined in etsy_service_mapping.h

    int quantity = 15;
    double price = 9.150;

    WhoMade whmd = we; // Defined in etsy_service_mapping.h

    WhenMade wnmd = made_to_order; // Defined in etsy_service_mapping.h

    unsigned long int shipping_template = 99999999999; // Max value an 11 digit number can hold

    //----------------------------------------------------------------------------------------------


    printf("-------------- Encoding Message ----------------------------\n\n");
    // Function call to for encoding the above paramters for etsy create_listing.
    // This function is defined in etsy.h
    EncodeForCreateListing(encodedMessage,13,t,d,quantity,price,whmd,wnmd,shipping_template,true);

    // printing the contents of the encoded message
    printf("Verifying Encoding Operation: \n\n");
    int i;
    for(i = 0;i<13;i++)
    {
        printf("ecnodedMessage[%d] = %u",i,(unsigned char)encodedMessage[i]);
        printf("\n");
    }


    printf("\n\n--------- Decoding Message ---------------------\n\n");

    // This function is defined in EtsyDecodeClientMessage.h
    DecodeEtsyClientMessage(encodedMessage,13);

    // Deallocating dynamically-allocated memories.
    // Otherwise, memory-leak would've occurred.
    free(encodedMessage);

   return 0;
}
