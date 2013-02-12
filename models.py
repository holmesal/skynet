from google.appengine.ext import db

class Arduino(db.Model):
	value = db.IntegerProperty()