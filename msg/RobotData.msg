std_msgs/Header header          # timestamp in the header is the acquisition time of 
                                # the first ray in the scan.
                                #
                                # in frame frame_id, angles are measured around 
                                # the positive Z axis (counterclockwise, if Z is up)
                                # with zero angle being forward along the x axis
uint32 boundingboxes_size       # 

PolarPoint[] boundingboxes      # 
geometry_msgs/Pose2D position   # odometry