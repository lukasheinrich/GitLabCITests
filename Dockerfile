#
# Image providing the project on top of an AnalysisBase image.
#

# Set up the base image:
FROM atlas/analysisbase:21.2.4

# Copy the project's sources into the image:
COPY CMakeLists.txt source/
COPY ZAnalysis source/ZAnalysis/

# Build the project inside a build/ directory:
RUN source /home/atlas/release_setup.sh && \
    mkdir build && cd build/ && cmake ../source/ && cmake --build . && cpack

# Install the created RPM as root:
USER root
RUN rpm -i /home/atlas/build/GitLabCITests_*.rpm

# Clean up:
RUN rm -rf /home/atlas/source/ /home/atlas/build/

# Switch back to the ATLAS account:
USER atlas

# Set up the environment setup script:
RUN mv release_setup.sh analysis_release_setup.sh
COPY docker_analysis_setup.sh /home/atlas/setup.sh

# Start the image with BASH by default:
CMD /bin/bash
