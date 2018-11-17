import smtplib
import pas
from email.mime.text import MIMEText
# from email.mime.image import MIMEImage
from email.mime.multipart import MIMEMultipart


# img=open('pic.jpg','rb').read()
def sendmail(temperture):
    msg=MIMEMultipart()

    msg['Subject']='Test Mail from Python'
    msg['From']='Fire Alarm'

    addr=['vigneshsrao5@gmail.com','tarunkant05@gmail.com', 'ashutosh.ahelleya@gmail.com','sidd.1105@gmail.com']

    msg['To']='vigneshsrao5@gmail.com'
    msg['cc']='ashutosh.ahelleya@gmail.com'

    body="Fire alarm at your house! Last recorded temprature = "+str(temperture)+" degrees Celius! Rush home now!"

    bdy=MIMEText(body,'plain')
    msg.attach(bdy)


    # image=MIMEImage(img,name=os.path.basename('pic.jpg'))

    # msg.attach(image)

    user='vigneshsrao5@gmail.com'
    passwd=pas.getpass()

    print("phase1 over____________")

    try:
        print("CONNECTING .....")
        s=smtplib.SMTP_SSL('smtp.gmail.com',465)
        print("HELLOING......")
        s.ehlo()
        print("TRYING TO LOGIN")
        s.login(user,passwd)
        print("LOGGED IN")
        s.sendmail('vigneshsrao5@gmail.com',addr,msg.as_string())
        s.close()
        print("Mail Sent")
    except Exception:
        print("An Error occured")
