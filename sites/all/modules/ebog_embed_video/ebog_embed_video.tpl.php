<div class="ebog_embed_video">


<?php fb($conf); ?>


  <?php if (!empty($conf['embed_youtube_code']) AND !empty($conf['embed_youtube_url'])) { ?>
    <p>youtube:</p>
    <div class="ebog_embed_youtube">
      <object style="width:207px;height:144px;">
        <param name="movie" value="http://www.youtube.com/v/<?php echo $conf['embed_youtube_code']; ?>">
        <param name="allowFullScreen" value="true">
        <param name="allowScriptAccess" value="always">
        <embed
          src="http://www.youtube.com/v/<?php echo $conf['embed_youtube_code']; ?>"
          type="application/x-shockwave-flash"
          allowfullscreen="true"
          allowScriptAccess="always"
          width="207"
          height="144"
        >
      </object>
    </div>
  <?php } ?>

  <?php if (!empty($conf['embed_vimeo_code']) AND !empty($conf['embed_vimeo_url'])) { ?>
    <p>vimeo:</p>
    <div class="ebog_embed_vimeo">
      <iframe 
        src="http://player.vimeo.com/video/<?php echo $conf['embed_vimeo_code']; ?>" 
        width="207"
        height="144"
        frameborder="0" 
        webkitAllowFullScreen 
        allowFullScreen
      >
      </iframe>
    </div>
  <?php } ?>

  <?php if ($conf['descr'] != '') { ?>
    <p>description:</p>
    <div class="ebog_embed_descr">
      <?php echo $conf['descr']; ?>
    </div>
  <?php } ?>


</div>
