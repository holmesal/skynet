import webapp2
import logging
import os


class HelloHandler(webapp2.RequestHandler):
	def get(self):
		
		logging.info(self.request.headers)
		logging.info(self.request.body)
		logging.info(self.request.remote_addr)

app = webapp2.WSGIApplication([
	('/hello',HelloHandler)
],debug=True)