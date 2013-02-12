import webapp2
import logging
import os
import jinja2

import models
from google.appengine.ext import db


jinja_environment = jinja2.Environment(loader=jinja2.FileSystemLoader(os.path.dirname(__file__)))

class AuthenticateHandler(webapp2.RequestHandler):
	def get(self):

		logging.info(self.request.body)

	def post(self):

		logging.info(self.request.body)
		


app = webapp2.WSGIApplication([
	('/authenticate',AuthenticateHandler)
],debug=True)