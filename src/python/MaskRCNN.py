#!/usr/bin/python
#Filename:MaskRCNN.py
import os
import sys
import random
import math
import numpy as np
import skimage.io
import matplotlib
import matplotlib.pyplot as plt

# import coco
# import utils
# import model as modellib
# import visualize

from maskrcnn_benchmark.config import cfg
from demo.predictor import COCODemo

class Mask:
	def __init__(self,device):
		print('Initializing Mask RCNN network...')
		config_file = "/usr/stud/linp/storage/user/linp/maskrcnn-benchmark/configs/caffe2/e2e_mask_rcnn_R_50_FPN_1x_caffe2.yaml"
		# "configs/caffe2/e2e_mask_rcnn_R_50_FPN_1x_caffe2.yaml"
		self.device = device
		# update the config options with the config file
		cfg.merge_from_file(config_file)
		# manual override some options
		cfg.merge_from_list(["MODEL.DEVICE", self.device])
		self.coco_demo = COCODemo(
			cfg,
			min_image_size=800,
			confidence_threshold=0.7,
		)
		print('Initialated Mask RCNN network...')

	def GetDynSeg(self,image,image2=None):
		image = image.astype(np.uint8)
		prediction = self.coco_demo.compute_prediction(image)
		top = self.coco_demo.select_top_predictions(prediction)
		masks = top.get_field("mask").numpy()
		h, w, c = image.shape
		rmask = np.zeros((h, w, 1)).astype(np.bool)
		for mask in masks:
			rmask |= mask[0, :, :, None]
		rmask = rmask.astype(np.uint8)
		return rmask

    
	

    


