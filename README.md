# Automotive Media Manager by Integrated Computer Solutions.
Read more about the architecture and the code on the ICS website blog series.
http://bit.ly/githubics-ivi-opensource

## Clone
Clone this repository with the --recursive option into a media-manager directory
to get all submodules
```
git clone --recursive git@gitlab.ics.com:media-manager/media-manager.git media-manager
```
This will create
```
media-manager
              core
              MediaInfoLib
              ZenLib
```

After that change into the media-manager directory and run
```
git checkout master
```
because by default all subrepos are stuck in detached HEAD

## Build
Simply open the main project file in QtCreator and build it.

The first time through you have to add `CONFIG+=build_all` to the qmake command.

After that you can remove this until the static libraries for ZenLib and
MediaInfoLib need to be rebuild.


