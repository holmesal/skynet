import webapp2
import logging
import os
import jinja2

import models
from google.appengine.ext import db

import json
import hashlib
import hmac

# import pusher


jinja_environment = jinja2.Environment(loader=jinja2.FileSystemLoader(os.path.dirname(__file__)))

class PusherAuthHandler(webapp2.RequestHandler):
	def post(self):
		
		#HARDCODED SECRET REMOVE THIS LATER
		app_key = 'e3d1f6a988eee7ce38e1'
		secret = '20537dec1073fa1dd82b'

		socket_id = self.request.get('socket_id')
		channel_name = self.request.get('channel_name')
		
		logging.info(socket_id)
		logging.info(channel_name)
		
		
		to_hash = socket_id + ":" + channel_name
		
		dig = hmac.new(secret, msg=to_hash, digestmod=hashlib.sha256).hexdigest()
		
		key_dig = app_key + ":" + dig
		
		response = {
			"auth"	:	key_dig
		}
		
		
		
		self.response.out.write(json.dumps(response))
		


app = webapp2.WSGIApplication([
	('/pusher/auth',PusherAuthHandler)
],debug=True)