
// 文件上传
var createFileUploader = function(FileUploader, parameter )
{

    // 检查参数类型是否正确

    // 文件上传数量
    var nFileCount = 0;
    
    // 上传文件中服务器上的存放路径列表
    var filePathList = [];

    var isError = false;

    var uploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: [ parameter ]
    });

    uploader.filters.push({
        name: 'customFilter',
        fn: function (item, options) 
        {
            return true;
            // var suffix = item.name.substring(item.name.lastIndexOf(".") + 1, item.name.length);
            // return suffix === 'xls' || suffix === 'xlsx' ? true : false;
        }
      });

    uploader.onAfterAddingAll = function(addedFileItems) 
    {
    };
    
    uploader.onBeforeUploadItem = function(item) 
    {
        console.info('onBeforeUploadItem', item);
    };

    uploader.onErrorItem = function (fileItem, response, status, headers) 
    {
        isError = true;
    };

    uploader.onCompleteItem = function(fileItem, response, status, headers)
    {
        fileItem.response = response;

        filePathList[nFileCount] = {
            file: response.file_path,
            type: response.file_type,
        };
    };

    uploader.onCompleteAll = function()
    {
        if ( isError )
        {
            MsgBox.info("提示", "文件上传失败！");
            return;
        }

        // 返回文件路径列表
        return filePathList;
    };

    return uploader;
};

