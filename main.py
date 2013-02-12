import webapp2
import logging
import os

import models
from google.appengine.ext import db


class MainHandler(webapp2.RequestHandler):
	def get(self):
		
		logging.info(self.request.headers)
		logging.info(self.request.body)
		logging.info(self.request.remote_addr)

class GetHandler(webapp2.RequestHandler):
	def get(self):
		
		arduino = models.Arduino.gql("").get()
		self.response.out.write('<value>'+str(arduino.value)+'</value>\n')
		
		
class CreateHandler(webapp2.RequestHandler):
	def get(self):
		
		existing = models.Arduino.gql("").get()
		
		if existing:
			self.response.out.write(existing.key())
		else:
			arduino = models.Arduino(value=0).put()
			self.response.out.write(arduino)

class SetHandler(webapp2.RequestHandler):
	def get(self):
		
		value = models.Arduino.gql("").get().value
		
		if value == 0:
			self.response.out.write('''
			<html><body><form action="/set" method="post">
			<input type="hidden" name="value" value="1">
			<input type="submit" value="Turn On"></body></html>
			''')
		else:
			self.response.out.write('''
			<html><body><form action="/set" method="post">
			<input type="hidden" name="value" value="0">
			<input type="submit" value="Turn Off"></body></html>
			''')
# 		
# 		self.response.out.write('''
# 		<form action="/set" method="post">
# 		<input type="number" name="value" min="0" max="1" step="1" value='''+str(value)+'''>
# 		<input type="submit">
# 		</form>
# 		''')
		
	def post(self):
		
		arduino = models.Arduino.gql("").get()
		arduino.value = int(self.request.get("value"))
		arduino.put()
		
		self.redirect('/set')


app = webapp2.WSGIApplication([
	('/',MainHandler),
	('/get',GetHandler),
	('/create',CreateHandler),
	('/set',SetHandler)
],debug=True)