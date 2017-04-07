from __future__ import print_function
import cv2

import analysis
import tracking
import bgs.pbas as pbas

if __name__ == '__main__':
    # noinspection PyArgumentList
    cap = cv2.VideoCapture('dataset/video.avi')
    if not cap.isOpened():
        print('Cannot open video!')
        exit(1)

    # noinspection PyUnresolvedReferences
    segmenter = pbas.PixelBasedAdaptiveSegmenter()
    # noinspection PyUnresolvedReferences
    counter = analysis.VehicleCouting()
    # noinspection PyUnresolvedReferences
    tracker = tracking.BlobTracking()

    while (True):
        ret, frame = cap.read()
        if not ret:
            break

        img_mask = segmenter.process(frame)
        if img_mask is not None:
            img_blob = tracker.process(frame, img_mask)
            counter.setInput(img_blob)
            counter.setTracks(tracker.getTracks())
            counter.process()

        cv2.imshow('input', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
