/*===================================================================================
*    Date : 2022/12/04(��)
*        Author		: Gakuto.S
*        File		: Load_Texture.h
*        Detail		:
===================================================================================*/
#pragma once

namespace common
{
	class LoadTexture
	{
	public:
		LoadTexture() = delete;
		LoadTexture(LoadTexture*) = delete;
		LoadTexture(LoadTexture&) = delete;

        //**************************************************
        /// \brief �R���X�g���N�^
        /// 
        /// \param[in] fileName
        /// 
        /// \return �Ȃ�
        //**************************************************
        LoadTexture(const char* fileName);

        //**************************************************
        /// \brief �R���X�g���N�^
        /// 
        /// \param[in] buffer
        /// 
        /// \return �Ȃ�
        //**************************************************
        LoadTexture(const unsigned char* buffer, int length);

        //**************************************************
        /// \brief �f�X�g���N�^
        /// 
        /// \return �Ȃ�
        //**************************************************
        ~LoadTexture();

        //**************************************************
        /// \brief �摜�̕����擾
        /// 
        /// \return �摜�̕�
        //**************************************************
        int GetWidth() { return m_width; }

        //**************************************************
        /// \brief �摜�̍������擾
        /// 
        /// \return �摜�̍���
        //**************************************************
        int GetHeight() { return m_height; }

        //**************************************************
        /// \brief �摜�̃f�[�^�̃A�h���X
        /// 
        /// \return �A�h���X
        //**************************************************
        unsigned char* GetImage() { return m_image; }

    private:
        unsigned char*  m_image;
        int             m_width;
        int             m_height;

    public:
        static constexpr int       k_RGBA = 4;
	};
}


