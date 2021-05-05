
@pushd %~dp0

@set Path=C:\Qt\4.7.3\bin;%Path%
@set Path=C:\Qt\mingw\bin;%Path%
@windres -o logo_res.o logo.rc
@qmake
@make release

@popd