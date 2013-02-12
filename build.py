import webapp2
import logging
import os
import jinja2

import models
from google.appengine.ext import db


jinja_environment = jinja2.Environment(loader=jinja2.FileSystemLoader(os.path.dirname(__file__)))

class BuildHandler(webapp2.RequestHandler):
	def get(self):

		template = jinja_environment.get_template('templates/choosetype.html')
		self.response.out.write(template.render())
		
class BuildHandler(webapp2.RequestHandler):
	def get(self):

		template = jinja_environment.get_template('templates/choosetype.html')
		self.response.out.write(template.render())
		
class ListenHandler(webapp2.RequestHandler):
	def get(self):
	
		template = jinja_environment.get_template('templates/listen.html')
		self.response.out.write(template.render())


class ActHandler(webapp2.RequestHandler):
	def get(self):
	
		template = jinja_environment.get_template('templates/act.html')
		self.response.out.write(template.render())


app = webapp2.WSGIApplication([
	('/build',BuildHandler),
	('/build/start',StartHandler),
	('/build/listen',ListenHandler),
	('/build/act',ActHandler)
],debug=True)