import webapp2
import logging
import os
import jinja2

import models
from google.appengine.ext import db


jinja_environment = jinja2.Environment(loader=jinja2.FileSystemLoader(os.path.dirname(__file__)))

class TestControllerHandler(webapp2.RequestHandler):
	def get(self):

		template = jinja_environment.get_template('templates/controller.html')
		self.response.out.write(template.render())
		


app = webapp2.WSGIApplication([
	('/test/controller',TestControllerHandler)
],debug=True)