(function ($) {
    
    Drupal.bibdkUpdateRating = function(voxb){
        $('.bibdk_voxb_tab[data-isbn=' + voxb.isbn + ']').html(voxb.rating.rating + voxb.rating.ratingCount + voxb.markup);
        Drupal.voxb_settings.init(voxb);
    }
    
    Drupal.bibdkGetRating = function(div){  
        var isbn = $(div).attr('data-isbn');    
        var request = $.ajax({
            url:Drupal.settings.basePath + 'voxb/ajax/update_rating',
            type:'POST',
            data:{
                isbn:isbn
            },
            dataType:'json',
            success:Drupal.bibdkUpdateRating
        });
       
    }
    
    
   
    Drupal.voxb_settings = {
        init: function(){            
            $('.voxb-rating.rate-enabled .rating').mouseover(function(e) {
                $(this).addClass('star-hover');
            });
            $('.voxb-rating.rate-enabled .rating').mouseout(function(e) {
                $(this).removeClass('star-hover');
            });
        }
    };
    
    Drupal.behaviors.bibdk_voxb_load = {
        attach: function (context) {
            $('.reviews', context).click(function (e) {
                var href= $(this).attr('href');
                var voxb_tab =  $(href).find('.bibdk_voxb_tab');     
                Drupal.bibdkGetRating(voxb_tab);                
            });
        }
    };
    
  
  
})(jQuery);