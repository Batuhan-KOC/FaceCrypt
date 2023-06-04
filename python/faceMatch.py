import getopt, sys

argumentList = sys.argv[1:]

options = "p:"

longOptions = ["image1=", "image2=", "port="]

try:
    # Parsing argument
    arguments, values = getopt.getopt(argumentList, options, longOptions)

    if True:
        im1Path = ""
        im2Path = ""
        portNumber = ""

        for argument, value in arguments:
            if argument in ("--image1"):
                im1Path = value
            elif argument in ("--image2"):
                im2Path = value
            elif argument in ("-p", "--port"):
                portNumber = value

        if((len(im1Path) == 0) | (len(im2Path) == 0) | (len(portNumber)) == 0):
            # output error
            print ("Invalid argument list error")
        else :
            print(("image 1 path    : %s") % im1Path)
            print(("image 2 path    : %s") % im2Path)
            print(("port number is  : %s") % portNumber)
            print("Importing dynamic libraries..")

            from deepface import DeepFace
            import socket

            print("Initializing udp socket..")
            UDP_IP = "127.0.0.1"
            UDP_PORT = int(portNumber)

            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            sock.bind((UDP_IP, UDP_PORT))

            print("Verifying images..")
            result = DeepFace.verify(img1_path = im1Path, img2_path = im2Path)

            print(result)
            
    else :
        # output error
        print ("Invalid argument list")
except getopt.error as err:
    # output error, and return with an error code
    print (str(err))