from genericpath import exists
import imghdr
from flask import Flask, render_template, send_file, request
import os

app = Flask(__name__)
file_name_variation = 0

# Route for "/" for a web-based interface to this micro-service:
@app.route('/')
def index():
  return render_template("index.html")


# Extract a hidden "uiuc" GIF from a PNG image:
@app.route('/extract', methods=["POST"])
def extract_hidden_gif():
  # ...your code here...
  if os.path.exists("VzFile") == False:
    os.mkdir("VzFile")
  os.system("make")
  global file_name_variation
  if os.path.exists("VVzFile") == False:
    os.mkdir("VVzFile")
  uploaded_file = request.files['png']
  if uploaded_file.filename != '':
        uploaded_file.save(f"VVzFile/{request.files['png'].filename}")
  if (imghdr.what(f"VVzFile/{request.files['png'].filename}") != "png"):
    return "Unprocessable Content", 422
  success = os.system(f"./png-extractGIF sample/{request.files['png'].filename} VzFile/{file_name_variation}.gif")

  
  if success == 0:
    uploaded_file = request.files['png']
    if uploaded_file.filename != '':
        uploaded_file.save(f"VzFile/{file_name_variation}.png")
    tmp = file_name_variation
    file_name_variation += 1
    return send_file(f"VzFile/{tmp}.gif", mimetype='image/gif'), 200
  return "File does not contain a hidden gif", 415

# Get the nth saved "uiuc" GIF:
@app.route('/extract/<int:image_num>', methods=['GET'])
def extract_image(image_num):
  if exists(f"VzFile/{image_num}.gif"):
    return send_file(f"VzFile/{image_num}.gif", mimetype='image/gif'), 200
  return "Not Founded", 404

  # ...
  
