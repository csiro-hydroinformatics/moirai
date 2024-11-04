# testmoirai

## Testing

`python run_tests.py`

## codegen

See the [wrapper generator c-api-wrapper-generation](https://github.com/csiro-hydroinformatics/c-api-wrapper-generation). The README.md has dotnet setup instructions, if need be.

Check you have dotnet installed `dotnet --info`

## Windows

### ongoing

```bat
%userprofile%\.venv\hydrofc\Scripts\activate

cd c:\src\moirai\tests\python_test_bindings
coverage run -m pytest
```

### One off install

Giving a try to the installation via powershell: https://github.com/pyenv-win/pyenv-win

restart a new PS or cmd

setting python 3.11.9 globally

```bat
pyenv --version
pyenv install -l
pyenv install 3.11.9
pyenv global 3.11.9
```

```bat
cd %userprofile% 
REM mkdir .venv
cd .venv
REM python -m venv hydrofc
.\hydrofc\Scripts\activate
```



```bat
REM pip install jupyterlab
REM python -m ipykernel install --user --name hydrofc --display-name "HFC"
cd %userprofile%\AppData\Local\Programs\SF\python
pip install refcount-1.2.2-py2.py3-none-any.whl
pip install cinterop-0.14.0-py2.py3-none-any.whl
pip install uchronia-2.4.1-py2.py3-none-any.whl
pip install swift2-2.4.8-py2.py3-none-any.whl
pip install fogss-2.4.2-py2.py3-none-any.whl
```